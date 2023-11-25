from __future__ import annotations
from typing import Literal

import goopylib.layout.align_offset_funcs as align_offset_funcs

_FLEX_WRAP_TYPE = Literal["nowrap", "wrap", "reverse"]
_FLEX_ALIGN_TYPE = Literal["start", "centre", "end", "space-around", "space-between", "space-evenly"]
_FLEX_CROSS_ITEM_ALIGN_TYPE = Literal["start", "centre", "end"]


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
                 cross_align: _FLEX_ALIGN_TYPE = "start",
                 cross_item_align: _FLEX_CROSS_ITEM_ALIGN_TYPE = "start") -> None:
        self._wrap = wrap
        self._align = align
        self._cross_align = cross_align
        self._cross_item_align = cross_item_align

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
        return self._cross_item_align

    @cross_align.setter
    def cross_align(self, value: _FLEX_ALIGN_TYPE):
        self._cross_item_align = value

    @property
    def cross_item_align(self) -> _FLEX_ALIGN_TYPE:
        return self._cross_item_align

    @cross_item_align.setter
    def cross_item_align(self, value: _FLEX_CROSS_ITEM_ALIGN_TYPE):
        self._cross_item_align = value

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

        container.content_box.width = container.padding_box.width - container.padding.x
        container.border_box.width = container.padding_box.width + container.border.x
        container.margin_box.width = container.border_box.width + container.margin.x

        if container.height.unit == "px":
            container.padding_box.height = container.height
        elif container.height.unit == "%":
            container.padding_box.height = container.height / 100 * container.parent.content_box.height
        elif container.height.unit == "auto":
            container.padding_box.height = container.padding.y + self.get_auto_height(container)
        else:
            raise ValueError()

        container.content_box.height = container.padding_box.height - container.padding.y
        container.border_box.height = container.padding_box.height + container.border.y
        container.margin_box.height = container.border_box.height + container.margin.y

        self._process_flex_items(container, _only_direct)

    def _process_flex_items(self, container: Container, _only_direct: bool) -> None:
        x = container.content_box.x1
        y = container.content_box.y1

        max_child_height = 0
        row_containers = []
        
        wrap = self._wrap != "nowrap"
        wrap_queue = []

        xspace = container.content_box.width
        yspace = container.content_box.height

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

                    self._align_wrap_queue(xspace, max_child_height, wrap_queue)
                    row_containers.append(wrap_queue)
                    wrap_queue = []

                    xspace = container.content_box.width
                    yspace -= max_child_height
                    max_child_height = 0

            if not _only_direct:
                child.layout.process_children(child, x, y, True)
            wrap_queue.append(child)

            if wrap:
                xspace -= child.margin_box.width
            max_child_height = max(max_child_height, child.margin_box.height)
            x = child.margin_box.x2

        self._align_wrap_queue(xspace, max_child_height, wrap_queue)

        row_containers.append(wrap_queue)
        self._vertical_align(yspace - max_child_height, row_containers)

        if not _only_direct:
            for child in container.children:
                child.layout.process_children(child, *child.margin_box.start)

    def _align_wrap_queue(self, whitespace: int, row_height: int, wrap_queue: list[Container]) -> None:
        self._horizontal_align_row(whitespace, wrap_queue)
        self._cross_item_align_wrap_queue(row_height, wrap_queue)

    def _horizontal_align_row(self, whitespace: int, items: list[Container]) -> None:
        if self._align == "start":
            return

        offset = align_offset_funcs._get_offset(self._align, whitespace, len(items))
        for i, child in enumerate(items):
            child.translate(offset(i), 0)

    def _vertical_align(self, whitespace: int, items: list[list[Container]]) -> None:
        if whitespace < 1:
            return
        if self._cross_align == "start":
            return

        offset = align_offset_funcs._get_offset(self._cross_align, whitespace, len(items))
        for i, row in enumerate(items):
            for child in row:
                child.translate(0, offset(i))

    def _cross_item_align_wrap_queue(self, row_height: int, wrap_queue: list[Container]) -> None:
        if self._cross_item_align == "start":
            return
        
        if self._cross_item_align == "centre":
            self._cross_item_align_wrap_queue_centre(row_height, wrap_queue)
        elif self._cross_item_align == "end":
            self._cross_item_align_wrap_queue_end(row_height, wrap_queue)

    @staticmethod
    def _cross_item_align_wrap_queue_centre(row_height: int, wrap_queue: list[Container]):
        for child in wrap_queue:
            child.translate(0, (row_height - child.margin_box.height) // 2)

    @staticmethod
    def _cross_item_align_wrap_queue_end(row_height: int, wrap_queue: list[Container]):
        for child in wrap_queue:
            child.translate(0, row_height - child.margin_box.height)

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

        if self._wrap != "nowrap" and container.width.unit != "auto":
            height = 0
            max_row_height = 0
            whitespace = container.content_box.width
            
            # TODO - Don't like this repeated code

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
                if whitespace < 0:
                    whitespace = container.content_box.width - width
                    height += max_row_height
                    max_row_height = 0

                max_row_height = max(max_row_height, child.border.y + child.margin.y
                                     + (child.height if child.height.unit == "px"
                                        else child.padding.y + child.layout.get_auto_height(child)))

            return height + max_row_height

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
