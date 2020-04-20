class MutationCursor:
    def __init__(self, root):
        self.node = root

    def __bool__(self):
        return self.node is not None

    def move(self, node):
        """Move the cursor to ``node``."""
        self.node = node
        return self

    def next(self):
        """Move the cursor to the next node."""

        if len(self.node):
            return self.move(self.node[0])

        if self.node.getnext() is not None:
            return self.move(self.node.getnext())

        parent = self.node.getparent()
        return self.move(parent.getnext())

    def divide(self, node=None):
        """
        Divide the parent of ``node`` at ``node``.

        The ``node``'s parent is duplicated and added to the tree immediately
        following the original. Then ``node`` and its following siblings are
        reparented into this duplicate. The parent node is always kept intact,
        even if it has no text and ``node`` is its first child.

        If ``node`` is ``None`` then the cursor's node will be used. In either
        case this operation doesn't move the cursor.

        For example if ``node`` is ``<target>`` in this tree:

        .. code-block:: xml

           <parent>
               prefix
               <target>text</target>
               suffix
           </parent>

        Then when ``<target>`` is divided this will become:

        .. code-block:: xml

           <parent>
               prefix
           </parent>
           <parent>
               <target>text</target>
               suffix
           </parent>

        This doesn't work on the root node or a direct child of the root node.
        """

        continuation = node.makeelement(node.getparent().tag,
                                        node.getparent().attrib,
                                        node.getparent().nsmap)
        continuation.extend([node] + list(node.itersiblings()))
        parent.addnext(continuation)

    def lift(self, node=None):
        """
        Lift ``node`` to become a sibling of its parent.

        If ``node`` has tail text or siblings following it, then its parent is
        duplicated and this duplicate is added to the tree immediately
        following the lifted ``node``. This duplicate is used as the parent of
        the tail text and the ``node``'s following siblings. The parent node is
        always kept intact, even if it has no text and ``node`` is its first
        child.

        If ``node`` is ``None`` then the cursor's node will be used. In either
        case this operation doesn't move the cursor.

        For example if ``node`` is ``<target>`` in this tree:

        .. code-block:: xml

           <parent>
               prefix
               <target>text</target>
               suffix
           </parent>

        Then when ``<target>`` is lifted this will become:

        .. code-block:: xml

           <parent>
               prefix
           </parent>
           <target>text</target>
           <parent>
               suffix
           </parent>

        This doesn't work on the root node or a direct child of the root node.
        """

        node = node if node is not None else cursor.node

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

    def remove(self, node=None):
        """
        Remove ``node`` from the document.

        If ``node`` has tail text then that text is reparented to a different
        node as appropriate.

        If ``node`` is ``None`` then the cursor's node will be used. In this
        case, or if ``node`` is an ancestor of the cursor's node, then the
        cursor is advanced before the node is removed. Otherwise this operation
        doesn't move the cursor.

        For example if ``node`` is ``None`` and the cursor is on ``<target>``
        in this tree:

        .. code-block:: xml

           <parent>
               prefix
               <one />
               <target>text</target>
               <two />
               suffix
           </parent>

        Then when ``<target>`` is removed this will become:

        .. code-block:: xml

           <parent>
               prefix
               <one />
               <two />
               suffix
           </parent>

        With the cursor on ``<two>``. Note that all descendants of the node to
        be removed are also removed.

        This doesn't work on the root node.
        """

        node = node if node is not None else cursor.node

        if node == cursor.node or node in set(cursor.node.iterancestors()):
            cursor.next()

        if node.tail:
            if node.getprevious() is not None:
                node.getprevious().tail = f"{node.getprevious().tail}{node.tail}"
            else:
                node.getparent().text = f"{node.getparent().text}{node.tail}"
        node.getparent().remove(node)
