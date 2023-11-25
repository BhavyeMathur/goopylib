from __future__ import annotations
from typing import Literal

_FLEX_WRAP_TYPE = Literal["nowrap", "wrap", "reverse"]
_FLEX_ALIGN_TYPE = Literal["start", "centre", "end", "space-around", "space-between", "space-evenly"]


class _LayoutMode:
    def process_children(self, container: Container, x: int, y: int, _only_direct: bool = False) -> None:
        raise NotImplemented()

    def get_auto_width(self, container: Container) -> int:
        raise NotImplemented()

    def get_auto_height(self, container: Container) -> int:
        raise NotImplemented()


class FlexLayout(_LayoutMode):
    def __init__(self,
                 wrap: _FLEX_WRAP_TYPE = "nowrap",
                 align: _FLEX_ALIGN_TYPE = "start",
                 cross_align: _FLEX_ALIGN_TYPE = "start") -> None:
        self._wrap = wrap
        self._align = align
        self._cross_align = cross_align

    @property
    def wrap(self) -> _FLEX_WRAP_TYPE:
        return self._wrap

    @wrap.setter
    def wrap(self, value: _FLEX_WRAP_TYPE):
        self._wrap = value

    @property
    def align(self) -> _FLEX_ALIGN_TYPE:
        return self._align

    @align.setter
    def align(self, value: _FLEX_ALIGN_TYPE):
        self._align = value

    @property
    def cross_align(self) -> _FLEX_ALIGN_TYPE:
        return self._cross_align

    @cross_align.setter
    def cross_align(self, value: _FLEX_ALIGN_TYPE):
        self._cross_align = value

    def process_children(self, container: Container, x: int, y: int, _only_direct: bool = False) -> None:
        container.margin_box.x1 = x
        container.margin_box.y1 = y

        container.border_box.x1 = x + container.margin.left
        container.border_box.y1 = y + container.margin.top

        container.padding_box.x1 = container.border_box.x1 + container.border.left
        container.padding_box.y1 = container.border_box.y1 + container.border.top

        container.content_box.x1 = container.padding_box.x1 + container.padding.left
        container.content_box.y1 = container.padding_box.y1 + container.padding.top

        if container.width.unit == "px":
            container.padding_box.width = container.width
        elif container.width.unit == "%":
            container.padding_box.width = container.width / 100 * container.parent.content_box.width
        elif container.width.unit == "auto":
            container.padding_box.width = container.padding.x + self.get_auto_width(container)
        else:
            raise ValueError()

        if container.height.unit == "px":
            container.padding_box.height = container.height
        elif container.height.unit == "%":
            container.padding_box.height = container.height / 100 * container.parent.content_box.height
        elif container.height.unit == "auto":
            print(1, _only_direct)
            container.padding_box.height = container.padding.y + self.get_auto_height(container)
        else:
            raise ValueError()

        container.content_box.width = container.padding_box.width - container.padding.x
        container.border_box.width = container.padding_box.width + container.border.x
        container.margin_box.width = container.border_box.width + container.margin.x

        container.content_box.height = container.padding_box.height - container.padding.y
        container.border_box.height = container.padding_box.height + container.border.y
        container.margin_box.height = container.border_box.height + container.margin.y

        self._process_flex_items(container, _only_direct)

    def _process_flex_items(self, container: Container, _only_direct: bool):
        x = container.content_box.x1
        y = container.content_box.y1

        max_child_height = 0
        wrap = self._wrap != "nowrap"
        wrap_queue = []

        whitespace = container.content_box.width

        for child in (container.children[::-1] if self._wrap == "reverse" else container.children):

            if wrap:
                if child.width.unit == "px":
                    width = child.width
                elif child.width.unit == "%":
                    width = (child.width * child.parent.content_box.width) // 100
                elif child.width.unit == "auto":
                    width = self.get_auto_width(child)
                else:
                    raise ValueError()
                width += child.margin.x + child.border.x

                if x + width > container.content_box.x2:
                    x = container.content_box.x1
                    y += max_child_height
                    max_child_height = 0

                    self._align_wrap_queue(whitespace, wrap_queue)
                    whitespace = container.content_box.width

            if not _only_direct:
                child.layout.process_children(child, x, y, True)
            wrap_queue.append(child)

            if wrap:
                max_child_height = max(max_child_height, child.margin_box.height)
                whitespace -= child.margin_box.width
            x = child.margin_box.x2

        self._align_wrap_queue(whitespace, wrap_queue)

        if not _only_direct:
            for child in container.children:
                child.layout.process_children(child, *child.margin_box.start)

    def _align_wrap_queue(self, whitespace: int, wrap_queue: list[Container]):
        if self.align == "start":
            return

        if self.align == "space-around":
            self._align_flex_wrap_queue_space_around(whitespace, wrap_queue)
        elif self.align == "space-between":
            self._align_flex_wrap_queue_space_between(whitespace, wrap_queue)
        elif self.align == "space-evenly":
            self._align_flex_wrap_queue_space_evenly(whitespace, wrap_queue)

        elif self.align == "centre":
            self._align_flex_wrap_queue_centre(whitespace, wrap_queue)
        elif self.align == "end":
            self._align_flex_wrap_queue_end(whitespace, wrap_queue)
            
        wrap_queue.clear()

    def _align_flex_wrap_queue_centre(self, whitespace, wrap_queue):
        xoffset = whitespace / 2
        for child in wrap_queue:
            child.translate(xoffset, 0)

    def _align_flex_wrap_queue_end(self, whitespace, wrap_queue):
        for child in wrap_queue:
            child.translate(whitespace, 0)

    def _align_flex_wrap_queue_space_around(self, whitespace, wrap_queue):
        xoffset = whitespace / (2 * len(wrap_queue))

        for i, child in enumerate(wrap_queue):
            child.translate((i * 2 + 1) * xoffset, 0)

    def _align_flex_wrap_queue_space_between(self, whitespace, wrap_queue):
        xoffset = whitespace / (len(wrap_queue) - 1)

        for i, child in enumerate(wrap_queue):
            child.translate(i * xoffset, 0)

    def _align_flex_wrap_queue_space_evenly(self, whitespace, wrap_queue):
        xoffset = whitespace / (len(wrap_queue) + 1)

        for i, child in enumerate(wrap_queue):
            child.translate((i + 1) * xoffset, 0)

    def get_auto_width(self, container: Container) -> int:
        if len(container.children) == 0:
            return 0

        return sum(child.border.x + child.margin.x +
                   (child.width
                    if child.width.unit == "px" else child.padding.x + child.layout.get_auto_width(child))
                   for child in container.children)

    def get_auto_height(self, container: Container) -> int:
        if len(container.children) == 0:
            return 0

        if self.wrap != "nowrap" and container.width.unit != "auto":
            height = 0
            max_row_height = 0
            whitespace = container.content_box.width

            print(container, whitespace)

            for child in container.children:
                if child.width.unit == "px":
                    width = child.width
                elif child.width.unit == "%":
                    width = (child.width * child.parent.content_box.width) // 100
                elif child.width.unit == "auto":
                    width = self.get_auto_width(child)
                else:
                    raise ValueError()

                whitespace -= width + child.margin.x + child.border.x
                # print("\t", whitespace)
                if whitespace < 0:
                    whitespace = container.content_box.width
                    height += max_row_height
                    max_row_height = 0

                max_row_height = max(max_row_height, child.border.y + child.margin.y
                                     + (child.height if child.height.unit == "px"
                                        else child.padding.y + child.layout.get_auto_height(child)))

            print(height)
            return height

        return max(child.border.y + child.margin.y +
                   (child.height
                    if child.height.unit == "px" else child.padding.y + child.layout.get_auto_height(child))
                   for child in container.children)


class FlowLayout(_LayoutMode):
    def __init__(self) -> None:
        pass

    def process_children(self, container: Container, x: int, y: int) -> None:
        pass


from .container import Container
