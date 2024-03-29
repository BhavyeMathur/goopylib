"""
Module defining colors listed by the W3CX11 specification
"""

from __future__ import annotations

from typing import Dict

# pylint: disable-next=no-name-in-module
from . import ColorHex


colors: Dict[str, ColorHex] = {
    "aliceblue":            ColorHex("f0f8ff"),
    "antiquewhite":         ColorHex("faebd7"),
    "aqua":                 ColorHex("00ffff"),
    "aquamarine":           ColorHex("7fffd4"),
    "azure":                ColorHex("f0ffff"),
    "beige":                ColorHex("f5f5dc"),
    "bisque":               ColorHex("ffe4c4"),
    "black":                ColorHex("000000"),
    "blanchedalmond":       ColorHex("ffebcd"),
    "blue":                 ColorHex("0000ff"),
    "blueviolet":           ColorHex("8a2be2"),
    "brown":                ColorHex("a52a2a"),
    "burlywood":            ColorHex("deb887"),
    "cadetblue":            ColorHex("5f9ea0"),
    "chartreuse":           ColorHex("7fff00"),
    "chocolate":            ColorHex("d2691e"),
    "coral":                ColorHex("ff7f50"),
    "cornflower":           ColorHex("6495ed"),
    "cornflowerblue":       ColorHex("6495ed"),
    "cornsilk":             ColorHex("fff8dc"),
    "crimson":              ColorHex("dc143c"),
    "cyan":                 ColorHex("00ffff"),
    "darkblue":             ColorHex("00008b"),
    "darkcyan":             ColorHex("008b8b"),
    "darkgoldenrod":        ColorHex("b8860b"),
    "darkgray":             ColorHex("a9a9a9"),
    "darkgreen":            ColorHex("006400"),
    "darkgrey":             ColorHex("a9a9a9"),
    "darkkhaki":            ColorHex("bdb76b"),
    "darkmagenta":          ColorHex("8b008b"),
    "darkolivegreen":       ColorHex("556b2f"),
    "darkorange":           ColorHex("ff8c00"),
    "darkorchid":           ColorHex("9932cc"),
    "darkred":              ColorHex("8b0000"),
    "darksalmon":           ColorHex("e9967a"),
    "darkseagreen":         ColorHex("8fbc8f"),
    "darkslateblue":        ColorHex("483d8b"),
    "darkslategray":        ColorHex("2f4f4f"),
    "darkslategrey":        ColorHex("2f4f4f"),
    "darkturquoise":        ColorHex("00ced1"),
    "darkviolet":           ColorHex("9400d3"),
    "deeppink":             ColorHex("ff1493"),
    "deepskyblue":          ColorHex("00bfff"),
    "dimgray":              ColorHex("696969"),
    "dimgrey":              ColorHex("696969"),
    "dodgerblue":           ColorHex("1e90ff"),
    "firebrick":            ColorHex("b22222"),
    "floralwhite":          ColorHex("fffaf0"),
    "forestgreen":          ColorHex("228b22"),
    "fuchsia":              ColorHex("ff00ff"),
    "gainsboro":            ColorHex("dcdcdc"),
    "ghostwhite":           ColorHex("f8f8ff"),
    "gold":                 ColorHex("ffd700"),
    "goldenrod":            ColorHex("daa520"),
    "gray":                 ColorHex("808080"),
    "green":                ColorHex("008000"),
    "greenyellow":          ColorHex("adff2f"),
    "grey":                 ColorHex("808080"),
    "honeydew":             ColorHex("f0fff0"),
    "hotpink":              ColorHex("ff69b4"),
    "indianred":            ColorHex("cd5c5c"),
    "indigo":               ColorHex("4b0082"),
    "ivory":                ColorHex("fffff0"),
    "khaki":                ColorHex("f0e68c"),
    "laserlemon":           ColorHex("ffff54"),
    "lavender":             ColorHex("e6e6fa"),
    "lavenderblush":        ColorHex("fff0f5"),
    "lawngreen":            ColorHex("7cfc00"),
    "lemonchiffon":         ColorHex("fffacd"),
    "lightblue":            ColorHex("add8e6"),
    "lightcoral":           ColorHex("f08080"),
    "lightcyan":            ColorHex("e0ffff"),
    "lightgoldenrod":       ColorHex("fafad2"),
    "lightgoldenrodyellow": ColorHex("fafad2"),
    "lightgray":            ColorHex("d3d3d3"),
    "lightgreen":           ColorHex("90ee90"),
    "lightgrey":            ColorHex("d3d3d3"),
    "lightpink":            ColorHex("ffb6c1"),
    "lightsalmon":          ColorHex("ffa07a"),
    "lightseagreen":        ColorHex("20b2aa"),
    "lightskyblue":         ColorHex("87cefa"),
    "lightslategray":       ColorHex("778899"),
    "lightslategrey":       ColorHex("778899"),
    "lightsteelblue":       ColorHex("b0c4de"),
    "lightyellow":          ColorHex("ffffe0"),
    "lime":                 ColorHex("00ff00"),
    "limegreen":            ColorHex("32cd32"),
    "linen":                ColorHex("faf0e6"),
    "magenta":              ColorHex("ff00ff"),
    "maroon":               ColorHex("800000"),
    "maroon2":              ColorHex("7f0000"),
    "maroon3":              ColorHex("b03060"),
    "mediumaquamarine":     ColorHex("66cdaa"),
    "mediumblue":           ColorHex("0000cd"),
    "mediumorchid":         ColorHex("ba55d3"),
    "mediumpurple":         ColorHex("9370db"),
    "mediumseagreen":       ColorHex("3cb371"),
    "mediumslateblue":      ColorHex("7b68ee"),
    "mediumspringgreen":    ColorHex("00fa9a"),
    "mediumturquoise":      ColorHex("48d1cc"),
    "mediumvioletred":      ColorHex("c71585"),
    "midnightblue":         ColorHex("191970"),
    "mintcream":            ColorHex("f5fffa"),
    "mistyrose":            ColorHex("ffe4e1"),
    "moccasin":             ColorHex("ffe4b5"),
    "navajowhite":          ColorHex("ffdead"),
    "navy":                 ColorHex("000080"),
    "oldlace":              ColorHex("fdf5e6"),
    "olive":                ColorHex("808000"),
    "olivedrab":            ColorHex("6b8e23"),
    "orange":               ColorHex("ffa500"),
    "orangered":            ColorHex("ff4500"),
    "orchid":               ColorHex("da70d6"),
    "palegoldenrod":        ColorHex("eee8aa"),
    "palegreen":            ColorHex("98fb98"),
    "paleturquoise":        ColorHex("afeeee"),
    "palevioletred":        ColorHex("db7093"),
    "papayawhip":           ColorHex("ffefd5"),
    "peachpuff":            ColorHex("ffdab9"),
    "peru":                 ColorHex("cd853f"),
    "pink":                 ColorHex("ffc0cb"),
    "plum":                 ColorHex("dda0dd"),
    "powderblue":           ColorHex("b0e0e6"),
    "purple":               ColorHex("800080"),
    "purple2":              ColorHex("7f007f"),
    "purple3":              ColorHex("a020f0"),
    "rebeccapurple":        ColorHex("663399"),
    "red":                  ColorHex("ff0000"),
    "rosybrown":            ColorHex("bc8f8f"),
    "royalblue":            ColorHex("4169e1"),
    "saddlebrown":          ColorHex("8b4513"),
    "salmon":               ColorHex("fa8072"),
    "sandybrown":           ColorHex("f4a460"),
    "seagreen":             ColorHex("2e8b57"),
    "seashell":             ColorHex("fff5ee"),
    "sienna":               ColorHex("a0522d"),
    "silver":               ColorHex("c0c0c0"),
    "skyblue":              ColorHex("87ceeb"),
    "slateblue":            ColorHex("6a5acd"),
    "slategray":            ColorHex("708090"),
    "slategrey":            ColorHex("708090"),
    "snow":                 ColorHex("fffafa"),
    "springgreen":          ColorHex("00ff7f"),
    "steelblue":            ColorHex("4682b4"),
    "tan":                  ColorHex("d2b48c"),
    "teal":                 ColorHex("008080"),
    "thistle":              ColorHex("d8bfd8"),
    "tomato":               ColorHex("ff6347"),
    "turquoise":            ColorHex("40e0d0"),
    "violet":               ColorHex("ee82ee"),
    "wheat":                ColorHex("f5deb3"),
    "white":                ColorHex("ffffff"),
    "whitesmoke":           ColorHex("f5f5f5"),
    "yellow":               ColorHex("ffff00"),
    "yellowgreen":          ColorHex("9acd32"),
}
