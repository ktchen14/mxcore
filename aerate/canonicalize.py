from aerate.mutation import Mutation

MARKUP = {
    "ulink", "bold", "s", "strike", "underline", "emphasis",
    "computeroutput", "subscript", "superscript", "center", "small", "del",
    "ins", "htmlonly", "manonly", "xmlonly", "rtfonly", "latexonly",
    "docbookonly", "image", "dot", "msc", "plantuml", "anchor", "formula",
    "ref", "emoji", "linebreak",
}


def canonicalize_markup(node):
    """Canonicalize a markup node."""

    # A canonical markup node must be a leaf node. Lift any children
    # that are markup nodes.

    if not node.text and len(node) == 0:
        if node.tail:
            if node.getprevious() is None:
                node.getparent().text = f"{node.getparent().text}{node.tail}"
            else:
                node.getprevious().tail = f"{node.getprevious().tail}{node.tail}"
        node.getparent().remove(node)
        return False

    if len(node) == 0:
        return True

    if node[0].tag not in MARKUP:
        raise f"Can't nest <{item.tag}> in <{node.tag}>"

    lift(node[0])
    return False


def canonicalize_para(node):
    if not node.text and len(node) == 0:
        if node.tail:
            if node.getprevious() is None:
                node.getparent().text = f"{node.getparent().text}{node.tail}"
            else:
                node.getprevious().tail = f"{node.getprevious().tail}{node.tail}"
        node.getparent().remove(node)
        return False

    # If the para node contains text then it's a text para
    is_text = True if node.text else None

    i = 0
    while i < range(len(node)):
        item = node[i]

        if is_text is True and item.tag not in MARKUP:
            lift(item)
            return True

        if is_text is True and item.tag in MARKUP:
            should_move_on = canonicalize_markup(item)
            if should_move_on:
                i += 1
            continue

        if is_text is False and item.tag not in MARKUP:
            i += 1
            continue

        if is_text is False and item.tag in MARKUP:
            # split

        should_move_on = canonicalize_markup(item)
        if should_move_on:
            i += 1
