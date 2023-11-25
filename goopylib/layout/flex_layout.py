from __future__ import annotations
from typing import Literal

from .layout_mode import _LayoutMode
from .container import Container

import goopylib.layout.align_offset_funcs as align_offset_funcs

_FLEX_WRAP_TYPE = Literal["nowrap", "wrap", "reverse"]
_FLEX_ALIGN_TYPE = Literal["start", "centre", "end", "space-around", "space-between", "space-evenly"]
_FLEX_ITEM_ALIGN_TYPE = Literal["start", "centre", "end"]


class _FlexManager:
    def __init__(self, container: Container, flex: Flex):
        self._div = container
        self._flex = flex
    
        self.x = self._div.content_box.x1
        self.y = self._div.content_box.y1

        self.max_child_height = 0
        self.row_containers = []

        self.wrap_queue = []

        self.xspace = self._div.content_box.width
        self.yspace = self._div.content_box.height
        
    def process_children(self, x: int, y: int, _only_direct: bool):
        self._div.margin_box.x1 = x
        self._div.margin_box.y1 = y

        self._div.border_box.x1 = x + self._div.margin.left
        self._div.border_box.y1 = y + self._div.margin.top

        self._div.padding_box.x1 = self._div.border_box.x1 + self._div.border.left
        self._div.padding_box.y1 = self._div.border_box.y1 + self._div.border.top

        self._div.content_box.x1 = self._div.padding_box.x1 + self._div.padding.left
        self._div.content_box.y1 = self._div.padding_box.y1 + self._div.padding.top

        if self._div.width.unit == "px":
            self._div.padding_box.width = self._div.width
        elif self._div.width.unit == "%":
            self._div.padding_box.width = self._div.width / 100 * self._div.parent.content_box.width
        elif self._div.width.unit == "auto":
            self._div.padding_box.width = self._div.padding.x + self._flex.get_auto_width(self._div)
        else:
            raise ValueError()

        self._div.content_box.width = self._div.padding_box.width - self._div.padding.x
        self._div.border_box.width = self._div.padding_box.width + self._div.border.x
        self._div.margin_box.width = self._div.border_box.width + self._div.margin.x

        if self._div.height.unit == "px":
            self._div.padding_box.height = self._div.height
        elif self._div.height.unit == "%":
            self._div.padding_box.height = self._div.height / 100 * self._div.parent.content_box.height
        elif self._div.height.unit == "auto":
            self._div.padding_box.height = self._div.padding.y + self.get_auto_height(self._div)
        else:
            raise ValueError()

        self._div.content_box.height = self._div.padding_box.height - self._div.padding.y
        self._div.border_box.height = self._div.padding_box.height + self._div.border.y
        self._div.margin_box.height = self._div.border_box.height + self._div.margin.y

        self.process_flex_items(_only_direct)
    
    def process_flex_items(self, _only_direct: bool):
        wrap = self._flex._wrap != "nowrap"
        self.__init__(self._div, self._flex)

        for child in (self._div.children[::-1] if self._flex._wrap == "reverse" else self._div.children):

            if wrap:
                if child.width.unit == "px":
                    width = child.width
                elif child.width.unit == "%":
                    width = (child.width * child.parent.content_box.width) // 100
                elif child.width.unit == "auto":
                    width = self._flex.get_auto_width(child)
                else:
                    raise ValueError()
                width += child.margin.x + child.border.x

                if self.x + width > self._div.content_box.x2:
                    self._end_row()

            if not _only_direct:
                child.layout.process_children(child, self.x, self.y, True)
            self.wrap_queue.append(child)

            if wrap:
                self.xspace -= child.margin_box.width
            self.max_child_height = max(self.max_child_height, child.margin_box.height)
            self.x = child.margin_box.x2

        self._end_row()
        self._vertical_align(self.yspace, self.row_containers)

        if not _only_direct:
            for child in self._div.children:
                child.layout.process_children(child, *child.margin_box.start)
    
    def _end_row(self) -> None:
        self.x = self._div.content_box.x1
        self.y += self.max_child_height

        self._align_row(self.xspace, self.max_child_height, self.wrap_queue)
        self.row_containers.append(self.wrap_queue)
        self.wrap_queue = []

        self.xspace = self._div.content_box.width
        self.yspace -= self.max_child_height
        self.max_child_height = 0

    def _align_row(self, whitespace: int, row_height: int, items: list[Container]) -> None:
        self._horizontal_align_row(whitespace, items)
        self._align_items_row(row_height, items)

    def _horizontal_align_row(self, whitespace: int, items: list[Container]) -> None:
        if self._flex._align == "start":
            return

        offset = align_offset_funcs._get_offset(self._flex._align, whitespace, len(items))
        for i, child in enumerate(items):
            child.translate(offset(i), 0)

    def _vertical_align(self, whitespace: int, items: list[list[Container]]) -> None:
        if whitespace < 1:
            return
        if self._flex._cross_align == "start":
            return

        offset = align_offset_funcs._get_offset(self._flex._cross_align, whitespace, len(items))
        for i, row in enumerate(items):
            for child in row:
                child.translate(0, offset(i))

    def _align_items_row(self, row_height: int, items: list[Container]) -> None:
        if self._flex._item_align == "start":
            return

        offset = align_offset_funcs._get_item_offset(self._flex._item_align, row_height)
        for child in items:
            child.translate(0, offset(child.margin_box.height))

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

        if self._flex._wrap != "nowrap" and container.width.unit != "auto":
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


class Flex(_LayoutMode):
    def __init__(self,
                 wrap: _FLEX_WRAP_TYPE = "nowrap",
                 align: _FLEX_ALIGN_TYPE = "start",
                 cross_align: _FLEX_ALIGN_TYPE = "start",
                 item_align: _FLEX_ITEM_ALIGN_TYPE = "start") -> None:
        self._wrap = wrap
        self._align = align
        self._cross_align = cross_align
        self._item_align = item_align

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
        return self._item_align

    @cross_align.setter
    def cross_align(self, value: _FLEX_ALIGN_TYPE):
        self._item_align = value

    @property
    def item_align(self) -> _FLEX_ALIGN_TYPE:
        return self._item_align

    @item_align.setter
    def item_align(self, value: _FLEX_ITEM_ALIGN_TYPE):
        self._item_align = value

    def process_children(self, container: Container, x: int, y: int, _only_direct: bool = False) -> None:
        _FlexManager(container, self).process_children(x, y, _only_direct)
