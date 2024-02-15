"""
Module defining colors
"""

from __future__ import annotations

from typing import Union

# pylint: disable-next=no-name-in-module, import-error
import goopylib.ext.color as _color


# The reason docstrings for the class are copied to the __init__() method is because CLion (and presumably PyCharm)
# renders the 'Raises' section with an odd indentation otherwise.

# The reason the RGBA properties are repeated in all the subclasses is because the Sphinx autodoc "orderby source"
# configuration doesn't seem to work with inherited members.


class Color:
    """
    Create colors by passing RGB arguments or a hexstring.
    Specify an alpha value by passing a float as the last parameter.

    Raises:
        ValueError: unrecognised or invalid parameters

    Examples:
        >>> Color(120, 70, 0)
        Color(120, 70, 0)

        >>> Color("#fff")
        Color(255, 255, 255)

        >>> Color(60, 180, 90, 0.5)
        Color(60, 180, 90)
    """

    __slots__ = ["_color"]

    def __init__(self, *args) -> None:
        """
        Create colors by passing RGB arguments or a hexstring.
        Specify an alpha value by passing a float as the last parameter.

        Raises:
            ValueError: unrecognised or invalid parameters

        Examples:
            >>> Color(120, 70, 0)
            Color(120, 70, 0)

            >>> Color("#fff")
            Color(255, 255, 255)

            >>> Color(60, 180, 90, 0.5)
            Color(60, 180, 90)
        """
        self._color = _color.Color(*args)

    def __repr__(self) -> str:
        return self._color.__repr__()

    def __add__(self, other: Union[int, Color]) -> Color:
        """
        Adds an integer value to the RGB components of the color,
        or adds the RGB components of another color supplied.
        You can also use in-place addition.

        Examples:
            >>> Color(120, 50, 60) + 5  # increases the brightness by 5 points
            Color(125, 55, 65)

            >>> Color(15, 20, 30) + Color(60, 5, 30)
            Color(75, 25, 60)
        """
        if isinstance(other, Color):
            return self._color.__add__(other._color)
        return self._color.__add__(other)

    def __sub__(self, other: Union[int, Color]) -> Color:
        """
        Subtracts an integer value from the RGB components of the color,
        or subtracts the RGB components of another color supplied.
        You can also use in-place subtraction.

        Examples:
            >>> Color(120, 50, 60) - 5  # decreases the brightness by 5 points
            Color(115, 45, 55)

            >>> Color(60, 80, 100) - Color(10, 20, 30)
            Color(50, 60, 70)
        """
        if isinstance(other, Color):
            return self._color.__sub__(other._color)
        return self._color.__sub__(other)

    def __iadd__(self, other: Union[int, Color]) -> None:
        if isinstance(other, Color):
            return self._color.__iadd__(other._color)
        return self._color.__iadd__(other)

    def __isub__(self, other: Union[int, Color]) -> None:
        if isinstance(other, Color):
            return self._color.__isub__(other._color)
        return self._color.__isub__(other)

    @property
    def red(self) -> int:
        """
        Returns:
            the red component of the color
        """
        return self._color.red

    @red.setter
    def red(self, value: int) -> None:
        self._color.red = value

    @property
    def green(self) -> int:
        """
        Returns:
            the green component of the color
        """
        return self._color.green

    @green.setter
    def green(self, value: int) -> None:
        self._color.green = value

    @property
    def blue(self) -> int:
        """
        Returns:
            the blue component of the color
        """
        return self._color.blue

    @blue.setter
    def blue(self, value: int) -> None:
        self._color.blue = value

    @property
    def alpha(self) -> float:
        """
        Returns:
            the alpha component of the color
        """
        return self._color.alpha

    @alpha.setter
    def alpha(self, value: float) -> None:
        self._color.alpha = value


class ColorRGB(Color):
    """
    Create an RGB color by passing RGB arguments with an optional alpha parameter.

    Convert another color object to RGB by passing it as a parameter.

    Raises:
        TypeError: expected integers for RGB and floats for alpha
        ValueError: RGB must be between 0-255, alpha must be between 0-1

    Examples:
        >>> ColorRGB(120, 70, 0)
        ColorRGB(120, 70, 0)

        >>> ColorRGB(60, 180, 90, 0.5)
        Color(60, 180, 90)

        >>> ColorRGB(other_color)
    """

    # pylint: disable-next=super-init-not-called
    def __init__(self, *args) -> None:
        """
        Create an RGB color by passing RGB arguments with an optional alpha parameter.

        Convert another color object to RGB by passing it as a parameter.

        Raises:
            TypeError: expected integers for RGB and floats for alpha
            ValueError: RGB must be between 0-255, alpha must be between 0-1

        Examples:
            >>> ColorRGB(120, 70, 0)
            ColorRGB(120, 70, 0)

            >>> ColorRGB(60, 180, 90, 0.5)
            Color(60, 180, 90)

            >>> ColorRGB(other_color)
        """
        self._color = _color.ColorRGB(*args)

    @property
    def red(self) -> int:
        """
        Returns:
            the red component of the color
        """
        return self._color.red

    @red.setter
    def red(self, value: int) -> None:
        self._color.red = value

    @property
    def green(self) -> int:
        """
        Returns:
            the green component of the color
        """
        return self._color.green

    @green.setter
    def green(self, value: int) -> None:
        self._color.green = value

    @property
    def blue(self) -> int:
        """
        Returns:
            the blue component of the color
        """
        return self._color.blue

    @blue.setter
    def blue(self, value: int) -> None:
        self._color.blue = value

    @property
    def alpha(self) -> float:
        """
        Returns:
            the alpha component of the color
        """
        return self._color.alpha

    @alpha.setter
    def alpha(self, value: float) -> None:
        self._color.alpha = value


class ColorHex(Color):
    """
    Create a Hexadecimal color by passing a hexstring with an optional alpha parameter. The '#' is optional.

    Convert another color object to Hexadecimal by passing it as a parameter.

    Raises:
        TypeError: hexstring must be a string, alpha must be a float

    Examples:
        >>> ColorHex("#000")
        "#000000"

        >>> ColorHex("a7b7c7")
        "#a7b7c7"

        >>> ColorHex(other_color)
    """

    # pylint: disable-next=super-init-not-called
    def __init__(self, *args) -> None:
        """
        Create a Hexadecimal color by passing a hexstring with an optional alpha parameter. The '#' is optional.

        Convert another color object to Hexadecimal by passing it as a parameter.

        Raises:
            TypeError: hexstring must be a string, alpha must be a float

        Examples:
            >>> ColorHex("#000")
            "#000000"

            >>> ColorHex("a7b7c7")
            "#a7b7c7"

            >>> ColorHex(other_color)
        """

        self._color = _color.ColorHex(*args)

    def __repr__(self) -> str:
        """
        Returns:
            the hexadecimal string
        """
        return self._color.__repr__()

    @property
    def red(self) -> int:
        """
        Returns:
            the red component of the color
        """
        return self._color.red

    @red.setter
    def red(self, value: int) -> None:
        self._color.red = value

    @property
    def green(self) -> int:
        """
        Returns:
            the green component of the color
        """
        return self._color.green

    @green.setter
    def green(self, value: int) -> None:
        self._color.green = value

    @property
    def blue(self) -> int:
        """
        Returns:
            the blue component of the color
        """
        return self._color.blue

    @blue.setter
    def blue(self, value: int) -> None:
        self._color.blue = value

    @property
    def alpha(self) -> float:
        """
        Returns:
            the alpha component of the color
        """
        return self._color.alpha

    @alpha.setter
    def alpha(self, value: float) -> None:
        self._color.alpha = value


class ColorCMYK(Color):
    """
    Create a CMYK color by passing cyan, magenta, yellow, key and optionally, alpha.

    Convert another color object to CMYK by passing it as a parameter.

    Raises:
        TypeError: CMYK, alpha values must be floats
        ValueError: CMYK, alpha values must be between 0-1

    Examples:
        >>> ColorCMYK(0.5, 0.4, 0.2, 0.1)
        ColorCMYK(0.5, 0.4, 0.2, 0.1)

        >>> ColorCMYK(0.6, 0.9, 1, 1, 0.5)
        ColorCMYK(0.6, 0.9, 1, 1)

        >>> ColorCMYK(other_color)
    """

    # pylint: disable-next=super-init-not-called
    def __init__(self, *args) -> None:
        """
        Create a CMYK color by passing cyan, magenta, yellow, key and optionally, alpha. All parameters are between 0-1.

        Convert another color object to CMYK by passing it as a parameter.

        Raises:
            TypeError: CMYK, alpha values must be floats
            ValueError: CMYK, alpha values must be between 0-1

        Examples:
            >>> ColorCMYK(0.5, 0.4, 0.2, 0.1)
            ColorCMYK(0.5, 0.4, 0.2, 0.1)

            >>> ColorCMYK(0.6, 0.9, 1, 1, 0.5)
            ColorCMYK(0.6, 0.9, 1, 1)

            >>> ColorCMYK(other_color)
        """
        self._color = _color.ColorCMYK(*args)

    @property
    def cyan(self) -> float:
        """
        Returns:
            the cyan component of the color
        """
        return self._color.cyan

    @cyan.setter
    def cyan(self, value: float) -> None:
        self._color.cyan = value

    @property
    def magenta(self) -> float:
        """
        Returns:
            the magenta component of the color
        """
        return self._color.magenta

    @magenta.setter
    def magenta(self, value: float) -> None:
        self._color.magenta = value

    @property
    def yellow(self) -> float:
        """
        Returns:
            the yellow component of the color
        """
        return self._color.yellow

    @yellow.setter
    def yellow(self, value: float) -> None:
        self._color.yellow = value

    @property
    def key(self) -> float:
        """
        Returns:
            the key component of the color
        """
        return self._color.key

    @key.setter
    def key(self, value: float) -> None:
        self._color.key = value

    @property
    def red(self) -> int:
        """
        Returns:
            the red component of the color
        """
        return self._color.red

    @red.setter
    def red(self, value: int) -> None:
        self._color.red = value

    @property
    def green(self) -> int:
        """
        Returns:
            the green component of the color
        """
        return self._color.green

    @green.setter
    def green(self, value: int) -> None:
        self._color.green = value

    @property
    def blue(self) -> int:
        """
        Returns:
            the blue component of the color
        """
        return self._color.blue

    @blue.setter
    def blue(self, value: int) -> None:
        self._color.blue = value

    @property
    def alpha(self) -> float:
        """
        Returns:
            the alpha component of the color
        """
        return self._color.alpha

    @alpha.setter
    def alpha(self, value: float) -> None:
        self._color.alpha = value


class ColorHSV(Color):
    """
    Create an HSV color by passing hue (0-360), saturation (0-1), value (0-1) and optionally, alpha (0-1)

    Convert another color object to HSV by passing it as a parameter.

    Raises:
        TypeError: hue must be an int, saturation, value, and alpha must be floats
        ValueError: hue must be between 0-360, saturation, value, and alpha must be between 0-1

    Examples:
        >>> ColorHSV(90, 0.2, 0.2)
        ColorHSV(90, 0.2, 0.2)

        >>> ColorHSV(240, 0.8, 0.9, 0.5)
        ColorHSV(240, 0.8, 0.9)

        >>> ColorHSV(other_color)
    """

    # pylint: disable-next=super-init-not-called
    def __init__(self, *args) -> None:
        """
        Create an HSV color by passing hue (0-360), saturation (0-1), value (0-1) and optionally, alpha (0-1)

        Convert another color object to HSV by passing it as a parameter.

        Raises:
            TypeError: hue must be an int, saturation, value, and alpha must be floats
            ValueError: hue must be between 0-360, saturation, value, and alpha must be between 0-1

        Examples:
            >>> ColorHSV(90, 0.2, 0.2)
            ColorHSV(90, 0.2, 0.2)

            >>> ColorHSV(240, 0.8, 0.9, 0.5)
            ColorHSV(240, 0.8, 0.9)

            >>> ColorHSV(other_color)
        """
        self._color = _color.ColorHSV(*args)

    @property
    def hue(self) -> int:
        """
        Returns:
            the hue component of the color
        """
        return self._color.hue

    @hue.setter
    def hue(self, value: int) -> None:
        self._color.hue = value

    @property
    def saturation(self) -> float:
        """
        Returns:
            the saturation component of the color
        """
        return self._color.saturation

    @saturation.setter
    def saturation(self, value: float) -> None:
        self._color.saturation = value

    @property
    def value(self) -> float:
        """
        Returns:
            the value component of the color
        """
        return self._color.value

    @value.setter
    def value(self, value: float) -> None:
        self._color.value = value

    @property
    def red(self) -> int:
        """
        Returns:
            the red component of the color
        """
        return self._color.red

    @red.setter
    def red(self, value: int) -> None:
        self._color.red = value

    @property
    def green(self) -> int:
        """
        Returns:
            the green component of the color
        """
        return self._color.green

    @green.setter
    def green(self, value: int) -> None:
        self._color.green = value

    @property
    def blue(self) -> int:
        """
        Returns:
            the blue component of the color
        """
        return self._color.blue

    @blue.setter
    def blue(self, value: int) -> None:
        self._color.blue = value

    @property
    def alpha(self) -> float:
        """
        Returns:
            the alpha component of the color
        """
        return self._color.alpha

    @alpha.setter
    def alpha(self, value: float) -> None:
        self._color.alpha = value


class ColorHSL(Color):
    """
    Create an HSL color by passing hue (0-360), saturation (0-1), luminance (0-1) and optionally, alpha (0-1)

    Convert another color object to HSL by passing it as a parameter.

    Raises:
        TypeError: hue must be an int, saturation, luminance, and alpha must be floats
        ValueError: hue must be between 0-360, saturation, luminance, and alpha must be between 0-1

    Examples:
        >>> ColorHSL(90, 0.2, 0.2)
        ColorHSL(90, 0.2, 0.2)

        >>> ColorHSL(240, 0.8, 0.9, 0.5)
        ColorHSL(240, 0.8, 0.9)

        >>> ColorHSL(other_color)
    """

    # pylint: disable-next=super-init-not-called
    def __init__(self, *args) -> None:
        """
        Create an HSL color by passing hue (0-360), saturation (0-1), luminance (0-1) and optionally, alpha (0-1)

        Convert another color object to HSL by passing it as a parameter.

        Raises:
            TypeError: hue must be an int, saturation, luminance, and alpha must be floats
            ValueError: hue must be between 0-360, saturation, luminance, and alpha must be between 0-1

        Examples:
            >>> ColorHSL(90, 0.2, 0.2)
            ColorHSL(90, 0.2, 0.2)

            >>> ColorHSL(240, 0.8, 0.9, 0.5)
            ColorHSL(240, 0.8, 0.9)

            >>> ColorHSL(other_color)
        """
        self._color = _color.ColorHSL(*args)

    @property
    def hue(self) -> int:
        """
        Returns:
            the hue component of the color
        """
        return self._color.hue

    @hue.setter
    def hue(self, value: int) -> None:
        self._color.hue = value

    @property
    def saturation(self) -> float:
        """
        Returns:
            the saturation component of the color
        """
        return self._color.saturation

    @saturation.setter
    def saturation(self, value: float) -> None:
        self._color.saturation = value

    @property
    def luminance(self) -> float:
        """
        Returns:
            the luminance component of the color
        """
        return self._color.luminance

    @luminance.setter
    def luminance(self, value: float) -> None:
        self._color.luminance = value

    @property
    def red(self) -> int:
        """
        Returns:
            the red component of the color
        """
        return self._color.red

    @red.setter
    def red(self, value: int) -> None:
        self._color.red = value

    @property
    def green(self) -> int:
        """
        Returns:
            the green component of the color
        """
        return self._color.green

    @green.setter
    def green(self, value: int) -> None:
        self._color.green = value

    @property
    def blue(self) -> int:
        """
        Returns:
            the blue component of the color
        """
        return self._color.blue

    @blue.setter
    def blue(self, value: int) -> None:
        self._color.blue = value

    @property
    def alpha(self) -> float:
        """
        Returns:
            the alpha component of the color
        """
        return self._color.alpha

    @alpha.setter
    def alpha(self, value: float) -> None:
        self._color.alpha = value


if not __debug__:
    # pylint: disable-next=no-name-in-module, import-error
    from goopylib.ext.color import *
    from _internal import convert_getset_descriptor_to_properties as _convert_getset_descriptor_to_properties

    _convert_getset_descriptor_to_properties(Color)
    _convert_getset_descriptor_to_properties(ColorRGB)
    _convert_getset_descriptor_to_properties(ColorHex)
    _convert_getset_descriptor_to_properties(ColorHSL)
    _convert_getset_descriptor_to_properties(ColorHSV)
    _convert_getset_descriptor_to_properties(ColorCMYK)
