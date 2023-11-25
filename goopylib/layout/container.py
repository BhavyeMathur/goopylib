from __future__ import annotations
from typing import Union, Literal, get_args


_UNITS = Literal["px", "%"]
_LRTB_SETTER_TYPE = Union[int, tuple[int], tuple[int, int], tuple[int, int, int], tuple[int, int, int, int]]


class _LRTB:
    def __init__(self, left: int, right: int, top: int, bottom: int):
        self._left = left
        self._right = right
        self._top = top
        self._bottom = bottom

        self._x = self._left + self._right
        self._y = self._top + self._bottom

    @property
    def left(self) -> int:
        return self._left

    @left.setter
    def left(self, value: int):
        self._left = value
        self._x = self.left + self._right

    @property
    def right(self) -> int:
        return self._right

    @right.setter
    def right(self, value: int):
        self._right = value
        self._x = self.left + self._right

    @property
    def top(self) -> int:
        return self._top

    @top.setter
    def top(self, value: int):
        self._top = value
        self._y = self._top + self._bottom

    @property
    def bottom(self) -> int:
        return self._bottom

    @bottom.setter
    def bottom(self, value: int):
        self._bottom = value
        self._y = self._top + self._bottom

    @property
    def x(self) -> int:
        return self._x

    @property
    def y(self) -> int:
        return self._y

    def set_values(self, value: _LRTB_SETTER_TYPE) -> None:
        if isinstance(value, int):
            self.left = value
            self.right = value
            self.top = value
            self.bottom = value

        elif len(value) == 1:
            self.set_values(value[0])

        elif len(value) == 2:
            self.top = value[0]
            self.bottom = value[0]
            self.left = value[1]
            self.right = value[1]

        elif len(value) == 3:
            self.top = value[0]
            self.right = value[1]
            self.left = value[1]
            self.bottom = value[2]

        elif len(value) == 4:
            self.top = value[0]
            self.right = value[1]
            self.bottom = value[2]
            self.left = value[3]


class _Box:
    def __init__(self) -> None:
        self._x1: int = 0
        self._x2: int = 0
        self._y1: int = 0
        self._y2: int = 0

        self._width: int = 0
        self._height: int = 0

    @property
    def x1(self) -> int:
        return self._x1

    @x1.setter
    def x1(self, value: int):
        self._x1 = value
        self._width = self._x2 - self._x1

    @property
    def y1(self) -> int:
        return self._y1

    @y1.setter
    def y1(self, value: int):
        self._y1 = value
        self._height = self._y2 - self._y1

    @property
    def x2(self) -> int:
        return self._x2

    @x2.setter
    def x2(self, value: int):
        self._x2 = value
        self._width = self._x2 - self._x1

    @property
    def y2(self) -> int:
        return self._y2

    @y2.setter
    def y2(self, value: int):
        self._y2 = value
        self._height = self._y2 - self._y1

    @property
    def width(self) -> int:
        return self._width

    @width.setter
    def width(self, value: int):
        self._x2 = self._x1 + value
        self._width = value

    @property
    def height(self) -> int:
        return self._height

    @height.setter
    def height(self, value: int):
        self._y2 = self._y1 + value
        self._height = value

    @property
    def start(self) -> tuple[int, int]:
        return self._x1, self._y1

    @property
    def end(self) -> tuple[int, int]:
        return self._x2, self._y2


class Dimension(int):
    def __new__(cls, value) -> int:
        x = int.__new__(cls, Dimension._parse_dimension(value)[0])
        return x

    def __init__(self, value):
        self._dimension, self._unit = Dimension._parse_dimension(value)
        super().__init__()

    @property
    def unit(self) -> _UNITS:
        return self._unit

    @staticmethod
    def _parse_dimension(dim: str | int) -> tuple[int, _UNITS]:
        if dim == "auto":
            return 0, "auto"
        
        if isinstance(dim, int):
            return dim, "px"

        for unit in get_args(_UNITS):
            if dim.endswith(unit):
                return int(dim[:-len(unit)]), unit

        raise ValueError(dim)


class Container:
    _context_tree: list[Container] = []
    _containers: list[Container] = []  # could consider making a dictionary

    def __init__(self, width, height, tag: str = "") -> None:
        self._width = Dimension(width)
        self._height = Dimension(height)

        self._margin = _LRTB(0, 0, 0, 0)
        self._border = _LRTB(0, 0, 0, 0)
        self._padding = _LRTB(0, 0, 0, 0)

        self._layout: layout_modes._LayoutMode = FlexLayout("nowrap", "start", "start")

        self._children: list[Container] = []
        self._parent: None | Container = Container._context_tree[-1] if len(Container._context_tree) > 0 else None
        if self._parent:
            self._parent._add_child(self)

        Container._containers.append(self)
        self._tag: str = tag

        # rendered attributes
        self._margin_box = _Box()
        self._border_box = _Box()
        self._padding_box = _Box()
        self._content_box = _Box()

        # debug attributes
        self.color: None | tuple[int, float, float] = None

    def __repr__(self) -> str:
        return f"Container({self._tag}) @ ({self._border_box.start})"

    def __enter__(self) -> Container:
        Container._context_tree.append(self)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb) -> None:
        Container._context_tree.pop()

    def _add_child(self, child) -> None:
        self._children.append(child)

    @property
    def margin_box(self) -> _Box:
        return self._margin_box

    @property
    def border_box(self) -> _Box:
        return self._border_box

    @property
    def padding_box(self) -> _Box:
        return self._padding_box

    @property
    def content_box(self) -> _Box:
        return self._content_box

    @property
    def margin(self) -> _LRTB:
        return self._margin

    @margin.setter
    def margin(self, value: _LRTB_SETTER_TYPE) -> None:
        self._margin.set_values(value)

    @property
    def padding(self) -> _LRTB:
        return self._padding

    @padding.setter
    def padding(self, value: _LRTB_SETTER_TYPE) -> None:
        self._padding.set_values(value)

    @property
    def border(self) -> _LRTB:
        return self._border

    @border.setter
    def border(self, value: _LRTB_SETTER_TYPE) -> None:
        self._border.set_values(value)

    @property
    def width(self) -> Dimension:
        return self._width

    @width.setter
    def width(self, value: int | str) -> None:
        self._width = Dimension(value)

    @property
    def height(self) -> Dimension:
        return self._height

    @height.setter
    def height(self, value: int | str) -> None:
        self._height = Dimension(value)

    @property
    def tag(self) -> str:
        return self._tag

    @property
    def children(self) -> tuple[Container, ...]:
        return tuple(self._children)

    @property
    def parent(self) -> Container | None:
        return self._parent

    @property
    def layout(self) -> layout_modes._LayoutMode:
        return self._layout

    @layout.setter
    def layout(self, value: layout_modes._LayoutMode) -> None:
        self._layout = value

    @staticmethod
    def get_containers() -> tuple[Container, ...]:
        return tuple(Container._containers)


from .layout_modes import *
import goopylib.layout.layout_modes as layout_modes