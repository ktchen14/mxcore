import collections.abc
import functools


class Mutation:
    def __init__(self, root):
        self.root = root
        self.i = -1
        self.seen = None

    @property
    def node(self):
        """Return the item's node or None if the item is the root's text."""

        if self.i == -1:
            return None
        return self.root[self.i // 2]

    @property
    def item(self):
        node = self.node
        if node is None:
            return self.root.text
        if self.i % 2 == 0:
            return node
        return node.text

    def skip(self):
        self.i += 1

    def next(self):
        while True:
            try:
                item = self.item
            except IndexError:
                raise StopIteration

            if item and item != self.seen:
                self.seen = item
                return item
            self.skip()


def lift(node):
    """
    Lift ``node`` one step up the tree to become a sibling of its parent.

    If ``node`` has tail text or siblings following it, then the parent node is
    duplicated. This duplicate is used as the parent of the tail text and
    following siblings. The parent node is always kept intact, even if it has
    no text and ``node`` is its first child.

    For example if ``node`` is ``<tolift>`` in this tree::

        <root>
            <parent>
                prefix
                <tolift>text</tolift>
                suffix
            </parent>
        </root>

    Then when lifted::

        <root>
            <parent>
                prefix
            </parent>
            <tolift>text</tolift>
            <parent>
                suffix
            </parent>
        </root>

    This doesn't work on the root node or a child of the root node.
    """

    parent = node.getparent()

    if node.tail or len(node.itersiblings()):
        continuation = node.makeelement(parent.tag,
                                        parent.attrib,
                                        parent.nsmap)
        continuation.text = node.tail
        node.tail = None
        continuation.extend(list(node.itersiblings()))

    parent.addnext(node)

    if continuation is not None:
        parent.addnext(continuation)


def canonicalize_markup(node):
    """Canonicalize a markup node."""

    # A canonical markup node should have only text. If it has any children
    # then pull them out of the node.

    PULLABLE = {
        "ulink", "bold", "s", "strike", "underline", "emphasis",
        "computeroutput", "subscript", "superscript", "center", "small", "del",
        "ins", "htmlonly", "manonly", "xmlonly", "rtfonly", "latexonly",
        "docbookonly", "image", "dot", "msc", "plantuml", "anchor", "formula",
        "ref", "emoji", "linebreak",
    }

    for i, item in enumerate(node):
        if item.tag in PULLABLE:
            lift(item)
            return True

        raise f"Can't nest <{item.tag}> in <{node.tag}>"
    return True


def canonicalize_para(node):
    MARKUP = {
        "ulink", "bold", "s", "strike", "underline", "emphasis",
        "computeroutput", "subscript", "superscript", "center", "small", "del",
        "ins", "htmlonly", "manonly", "xmlonly", "rtfonly", "latexonly",
        "docbookonly", "image", "dot", "msc", "plantuml", "anchor", "formula",
        "ref", "emoji", "linebreak",
    }

    # If the para node contains text then it's a text para
    is_text = True if node.text else None

    i = 0
    while i < range(len(node)):
        item = node[i]

        if is_text is True and item.tag not in MARKUP:
            lift(item)
            return True

        if is_text is False and item.tag in MARKUP:
            pass

        should_move_on = canonicalize_markup(item)
        if should_move_on:
            i += 1

class InlineMarkup:
    def __init__(self, node):
        self.node = node


class ParaRenderer:
    def gather_inline_node(self, node) -> List[Union[str, InlineMarkup]]:
        result = []

        text = node.xpath("string()")
        if text:
            result.append(InlineMarkup(text))

        tail = node.tail
        if tail:
            result.append(tail)

        return result

    def gather(self, node) -> List[Union[str, InlineMarkup]]:
        result = [node.text] if node.text else []
        for item in node.iterchildren():
            result.extend(self.gather_inline_node(item))
        return result

    def render(self, node):
        def adjoin(result, item):
            if not result:
                return [item]

            if isinstance(item, str) and isinstance(result[-1], str):
                result[-1] += item
            else:
                result.append(item)

            return result

        result = functools.reduce(adjoin, self.gather(node), [])
