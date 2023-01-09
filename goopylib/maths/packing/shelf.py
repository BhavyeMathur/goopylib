"""
Module defining rectangle packing using shelf-based algorithms.

See https://pds25.egloos.com/pds/201504/21/98/RectangleBinPack.pdf

"""

from __future__ import annotations

from typing import Optional, Callable

from .packing import PackingAlgorithm, SortingFunction, Item, Bin, ShelvedBin, sort_by_long_side, sort_by_short_side


# Bin shadows the Python built-in 'bin', but until I can think
# of a better name to call it, I'm going to stick with bin.
# pylint: disable=redefined-builtin


class Shelf:
    """
    Class representing a single shelf (row) for shelf-based packing algorithms

    Args:
        vertical_offset: the vertical position of the shelf
        bin: the bin associated with the shelf
    """

    def __init__(self, vertical_offset: float, bin: ShelvedBin) -> None:
        """
        Class representing a single shelf (row) for shelf-based packing algorithms

        Args:
            vertical_offset: the vertical position of the shelf
            bin: the bin associated with the shelf
        """

        self.width: float = bin.width
        self.height: float = 0

        self.packed_width: float = 0
        self.available_width: float = self.width

        self.vertical_offset: float = vertical_offset

        self.is_open = True
        self.bin: Bin = bin
        self.items: list[Item] = []

    def __repr__(self) -> str:
        return f"Shelf(offset={self.vertical_offset}, packed={self.packed_width}) with {len(self.items)} items"

    def _fits(self, item: Item) -> bool:
        """
        Returns:
            if an item fits inside this shelf.
        """
        if self.is_open:
            return item.width <= self.available_width and self.vertical_offset + item.height <= self.bin.height
        else:
            return item.width <= self.available_width and item.height <= self.height

    def _fits_above(self, item: Item) -> bool:
        """
        Returns:
            if an item will fit on a level above this shelf.
        """
        return self.vertical_offset + self.height + item.height <= self.bin.height

    def _add(self, item: Item) -> None:
        """
        Adds an item to this shelf.
        """
        item._x = self.packed_width
        item._y = self.vertical_offset
        self.bin._add(item)
        self.items.append(item)

        if item.height > self.height:
            self.height = item.height

        self.packed_width += item.width
        self.available_width -= item.width

    def _close(self) -> None:
        """
        Closes this shelf and locks its height.
        """
        self.is_open = False

    def packed_area(self) -> float:
        """
        Returns:
            the amount of area that has been packed into this shelf
        """
        if self.is_open:
            return self.packed_width * self.height
        else:
            return self.width * self.height


# Shelf Packing Algorithms


class ShelfPackingAlgorithm(PackingAlgorithm):
    """
    A class representing a shelf-based packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """
        super().__init__(bin_width, bin_height)
        self._bins: list[ShelvedBin] = [ShelvedBin(width=bin_width, height=bin_height)]

    def pack(self, item: Item, allow_rotation: bool = True) -> None:
        """
        Packs a rectangular item using a shelf-based algorithm.

        Args:
            item: the item to pack
            allow_rotation: whether to allow rotating the item
        """
        raise NotImplementedError()

    def pack_all(self,
                 items: list[Item],
                 sorting: Optional[SortingFunction] = sort_by_short_side(True),
                 allow_rotation: bool = True) -> None:
        """
        Packs a list of rectangular items into bins.

        Args:
            items: the list of items
            sorting: a sorting function to use, otherwise None.
            allow_rotation: whether to allow rotating the items
        """
        PackingAlgorithm.pack_all(self, items, sorting, allow_rotation=allow_rotation)

    def pack_oriented(self,
                      item: Item,
                      orient_vertically: bool = True) -> None:
        """
        Packs a rectangular item oriented in a particular way using this algorithm.

        Args:
            item: the item to pack
            orient_vertically: whether to orient the item vertically
        """
        if item.is_horizontal() != orient_vertically:
            item._rotate()
        self.pack(item, allow_rotation=False)

    def pack_all_oriented(self,
                          items: list[Item],
                          sorting: Optional[SortingFunction] = sort_by_long_side(descending=True),
                          orient_vertically: bool = True) -> None:
        """
        Packs a list of rectangular items into bins oriented in a particular way.

        Args:
            items: the list of items
            sorting: a sorting function to use, otherwise None.
            orient_vertically: whether to orient the items vertically
        """
        if sorting:
            items = sorting(items)

        for item in items:
            self.pack_oriented(item, orient_vertically=orient_vertically)


class NextFit(ShelfPackingAlgorithm):
    """
    A class representing a shelf-based, next-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, next-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """

        super().__init__(bin_width, bin_height)
        self._shelf = self._bins[-1].open_shelf

    def pack(self, item: Item, allow_rotation: bool = True) -> None:
        """
        Packs a rectangular item using a shelf-based next-fit algorithm.

        Args:
            item: the item to pack
            allow_rotation: whether to allow rotating the item
        """
        if allow_rotation and (item.is_vertical() != (item.long_side <= self._shelf.height)):
            item._rotate()

        if self._shelf._fits(item):
            self._shelf._add(item)
            return

        if self._shelf._fits_above(item):
            self._shelf = self._bins[-1]._add_shelf()
        else:
            self._bins.append(ShelvedBin(width=self._bin_width, height=self._bin_height))
            self._shelf = self._bins[-1].open_shelf

        if allow_rotation and item.is_vertical():
            item._rotate()
        self._shelf._add(item)


class FirstFit(ShelfPackingAlgorithm):
    """
    A class representing a shelf-based, first-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, first-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """

        super().__init__(bin_width, bin_height)
        self._shelf = self._bins[-1].open_shelf

    def pack(self, item: Item, allow_rotation: bool = True) -> None:
        """
        Packs a rectangular item using a shelf-based first-fit algorithm.

        Args:
            item: the item to pack
            allow_rotation: whether to allow rotating the item
        """

        def _add_to_bin() -> bool:
            for shelf in bin:
                if allow_rotation and (item.is_vertical() != (item.long_side <= shelf.height)):
                    item._rotate()

                if shelf._fits(item):
                    shelf._add(item)
                    return True

            if shelf._fits_above(item):  # noqa W0631, each bin has at least 1 shelf
                if allow_rotation and item.is_vertical():
                    item._rotate()
                bin._add_shelf()._add(item)
                return True

            return False

        for bin in self._bins:
            if _add_to_bin():
                break
        else:
            self._bins.append(ShelvedBin(width=self._bin_width, height=self._bin_height))

            if allow_rotation and item.is_vertical():
                item._rotate()
            self._bins[-1].open_shelf._add(item)


class ScoredFit(ShelfPackingAlgorithm):
    """
    A class representing a shelf-based, scored-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float, scoring_function: Callable[[Shelf, Item], float]):
        """
        A class representing a shelf-based, scored-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
            scoring_function: the scoring function to compare shelf candidates
        """
        super().__init__(bin_width, bin_height)
        self._shelf = self._bins[-1].open_shelf
        self._scoring_func = scoring_function

    def pack(self, item: Item, allow_rotation: bool = True) -> None:
        """
        Packs a rectangular item using a shelf-based, scored-fit algorithm.

        Args:
            item: the item to pack
            allow_rotation: whether to allow rotating the item
        """

        def _score(shelf, obj) -> None:
            nonlocal best_shelf, best_score, best_orientation

            score = self._scoring_func(shelf, obj)
            if score > best_score:
                best_shelf = shelf
                best_score = score
                best_orientation = item.is_rotated()

        def _add_to_bin() -> None:
            for shelf in bin:
                if allow_rotation and (item.is_vertical() != (item.long_side <= shelf.height)):
                    item._rotate()

                if shelf._fits(item):
                    _score(shelf, item)

            # pylint: disable-next=undefined-loop-variable
            if best_shelf is None and shelf._fits_above(item):  # noqa W0631, each bin has at least 1 shelf
                shelf = bin._add_shelf()

                if allow_rotation and item.is_vertical():
                    item._rotate()

                _score(shelf, item)

        best_shelf = None
        best_score = float("-inf")
        best_orientation = False  # un-rotated

        for bin in self._bins:
            _add_to_bin()

        if best_shelf is None:
            self._bins.append(ShelvedBin(width=self._bin_width, height=self._bin_height))

            if allow_rotation and item.is_vertical():
                item._rotate()

            best_shelf = self._bins[-1].open_shelf
            best_orientation = item.is_rotated()

        if item.is_rotated() != best_orientation:
            item._rotate()

        best_shelf._add(item)


class BestWidthFit(ScoredFit):
    """
    A class representing a shelf-based, best-width-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, best-width-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """
        super().__init__(bin_width, bin_height, lambda shelf, obj: obj.width - shelf.available_width)


class WorstWidthFit(ScoredFit):
    """
    A class representing a shelf-based, worst-width-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, best-width-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """
        super().__init__(bin_width, bin_height, lambda shelf, obj: shelf.available_width - obj.width)


class BestHeightFit(ScoredFit):
    """
    A class representing a shelf-based, best-height-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, best-height-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """
        super().__init__(bin_width, bin_height, lambda shelf, obj: obj.height - shelf.available_height)


class WorstHeightFit(ScoredFit):
    """
    A class representing a shelf-based, worst-height-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, worst-height-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """
        super().__init__(bin_width, bin_height, lambda shelf, obj: shelf.available_height - obj.height)


class BestAreaFit(ScoredFit):
    """
    A class representing a shelf-based, best-area-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, best-area-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """
        super().__init__(bin_width, bin_height,
                         lambda shelf, obj: (shelf.packed_width + obj.width) * max(obj.height, shelf.height))


class WorstAreaFit(ScoredFit):
    """
    A class representing a shelf-based, worst-area-fit packing algorithm.

    Args:
        bin_width: the maximum width of the packed bin
        bin_height: the maximum height of the packed bin
    """

    def __init__(self, bin_width: float, bin_height: float):
        """
        A class representing a shelf-based, worst-area-fit packing algorithm.

        Args:
            bin_width: the maximum width of the packed bin
            bin_height: the maximum height of the packed bin
        """
        super().__init__(bin_width, bin_height,
                         lambda shelf, obj: -(shelf.packed_width + obj.width) * max(obj.height, shelf.height))

# pylint: enable=redefined-builtin
