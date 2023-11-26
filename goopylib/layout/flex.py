from __future__ import annotations
from typing import Literal


_FLEX_WRAP_TYPE = Literal["nowrap", "wrap", "reverse"]
_FLEX_ALIGN_TYPE = Literal["start", "centre", "end", "space-around", "space-between", "space-evenly"]
_FLEX_ITEM_ALIGN_TYPE = Literal["start", "centre", "end"]


class Flex:
    def __init__(self,
                 wrap: _FLEX_WRAP_TYPE = "nowrap",
                 align: _FLEX_ALIGN_TYPE = "start",
                 cross_align: _FLEX_ALIGN_TYPE = "start",
                 item_align: _FLEX_ITEM_ALIGN_TYPE = "start",
                 row_gap: int = 0,
                 column_gap: int = 0,
                 order: int = 0) -> None:
        self._wrap = wrap
        self._align = align
        self._cross_align = cross_align
        self._item_align = item_align
        self._order: int = order
        # TODO self._direction: str
        # TODO self._grow: float
        # TODO self._cross_align_self: _FLEX_ALIGN_TYPE
        self._row_gap: int = row_gap
        self._column_gap: int = column_gap

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

    @property
    def row_gap(self) -> int:
        return self._row_gap

    @row_gap.setter
    def row_gap(self, value: int):
        self._row_gap = value

    @property
    def column_gap(self) -> int:
        return self._column_gap

    @column_gap.setter
    def column_gap(self, value: int):
        self._column_gap = value

    @property
    def order(self) -> int:
        return self._order

    @order.setter
    def order(self, value: int):
        self._order = value
