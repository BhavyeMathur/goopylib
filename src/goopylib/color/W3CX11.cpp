#define GP_LOGGING_LEVEL 3
#include "W3CX11.h"


namespace gp {
    GPAPI std::unordered_map<std::string, Scope<ColorHex>> colors;

    void initW3CX11() {
        GP_CORE_DEBUG("gp::initW3CX11()");

        colors["aliceblue"] = CreateScope<ColorHex>("f0f8ff");
        colors["antiquewhite"] = CreateScope<ColorHex>("faebd7");
        colors["aqua"] = CreateScope<ColorHex>("00ffff");
        colors["aquamarine"] = CreateScope<ColorHex>("7fffd4");
        colors["azure"] = CreateScope<ColorHex>("f0ffff");
        colors["beige"] = CreateScope<ColorHex>("f5f5dc");
        colors["bisque"] = CreateScope<ColorHex>("ffe4c4");
        colors["black"] = CreateScope<ColorHex>("000000");
        colors["blanchedalmond"] = CreateScope<ColorHex>("ffebcd");
        colors["blue"] = CreateScope<ColorHex>("0000ff");
        colors["blueviolet"] = CreateScope<ColorHex>("8a2be2");
        colors["brown"] = CreateScope<ColorHex>("a52a2a");
        colors["burlywood"] = CreateScope<ColorHex>("deb887");
        colors["cadetblue"] = CreateScope<ColorHex>("5f9ea0");
        colors["chartreuse"] = CreateScope<ColorHex>("7fff00");
        colors["chocolate"] = CreateScope<ColorHex>("d2691e");
        colors["coral"] = CreateScope<ColorHex>("ff7f50");
        colors["cornflower"] = CreateScope<ColorHex>("6495ed");
        colors["cornflowerblue"] = CreateScope<ColorHex>("6495ed");
        colors["cornsilk"] = CreateScope<ColorHex>("fff8dc");
        colors["crimson"] = CreateScope<ColorHex>("dc143c");
        colors["cyan"] = CreateScope<ColorHex>("00ffff");
        colors["darkblue"] = CreateScope<ColorHex>("00008b");
        colors["darkcyan"] = CreateScope<ColorHex>("008b8b");
        colors["darkgoldenrod"] = CreateScope<ColorHex>("b8860b");
        colors["darkgray"] = CreateScope<ColorHex>("a9a9a9");
        colors["darkgreen"] = CreateScope<ColorHex>("006400");
        colors["darkgrey"] = CreateScope<ColorHex>("a9a9a9");
        colors["darkkhaki"] = CreateScope<ColorHex>("bdb76b");
        colors["darkmagenta"] = CreateScope<ColorHex>("8b008b");
        colors["darkolivegreen"] = CreateScope<ColorHex>("556b2f");
        colors["darkorange"] = CreateScope<ColorHex>("ff8c00");
        colors["darkorchid"] = CreateScope<ColorHex>("9932cc");
        colors["darkred"] = CreateScope<ColorHex>("8b0000");
        colors["darksalmon"] = CreateScope<ColorHex>("e9967a");
        colors["darkseagreen"] = CreateScope<ColorHex>("8fbc8f");
        colors["darkslateblue"] = CreateScope<ColorHex>("483d8b");
        colors["darkslategray"] = CreateScope<ColorHex>("2f4f4f");
        colors["darkslategrey"] = CreateScope<ColorHex>("2f4f4f");
        colors["darkturquoise"] = CreateScope<ColorHex>("00ced1");
        colors["darkviolet"] = CreateScope<ColorHex>("9400d3");
        colors["deeppink"] = CreateScope<ColorHex>("ff1493");
        colors["deepskyblue"] = CreateScope<ColorHex>("00bfff");
        colors["dimgray"] = CreateScope<ColorHex>("696969");
        colors["dimgrey"] = CreateScope<ColorHex>("696969");
        colors["dodgerblue"] = CreateScope<ColorHex>("1e90ff");
        colors["firebrick"] = CreateScope<ColorHex>("b22222");
        colors["floralwhite"] = CreateScope<ColorHex>("fffaf0");
        colors["forestgreen"] = CreateScope<ColorHex>("228b22");
        colors["fuchsia"] = CreateScope<ColorHex>("ff00ff");
        colors["gainsboro"] = CreateScope<ColorHex>("dcdcdc");
        colors["ghostwhite"] = CreateScope<ColorHex>("f8f8ff");
        colors["gold"] = CreateScope<ColorHex>("ffd700");
        colors["goldenrod"] = CreateScope<ColorHex>("daa520");
        colors["gray"] = CreateScope<ColorHex>("808080");
        colors["green"] = CreateScope<ColorHex>("008000");
        colors["greenyellow"] = CreateScope<ColorHex>("adff2f");
        colors["grey"] = CreateScope<ColorHex>("808080");
        colors["honeydew"] = CreateScope<ColorHex>("f0fff0");
        colors["hotpink"] = CreateScope<ColorHex>("ff69b4");
        colors["indianred"] = CreateScope<ColorHex>("cd5c5c");
        colors["indigo"] = CreateScope<ColorHex>("4b0082");
        colors["ivory"] = CreateScope<ColorHex>("fffff0");
        colors["khaki"] = CreateScope<ColorHex>("f0e68c");
        colors["laserlemon"] = CreateScope<ColorHex>("ffff54");
        colors["lavender"] = CreateScope<ColorHex>("e6e6fa");
        colors["lavenderblush"] = CreateScope<ColorHex>("fff0f5");
        colors["lawngreen"] = CreateScope<ColorHex>("7cfc00");
        colors["lemonchiffon"] = CreateScope<ColorHex>("fffacd");
        colors["lightblue"] = CreateScope<ColorHex>("add8e6");
        colors["lightcoral"] = CreateScope<ColorHex>("f08080");
        colors["lightcyan"] = CreateScope<ColorHex>("e0ffff");
        colors["lightgoldenrod"] = CreateScope<ColorHex>("fafad2");
        colors["lightgoldenrodyellow"] = CreateScope<ColorHex>("fafad2");
        colors["lightgray"] = CreateScope<ColorHex>("d3d3d3");
        colors["lightgreen"] = CreateScope<ColorHex>("90ee90");
        colors["lightgrey"] = CreateScope<ColorHex>("d3d3d3");
        colors["lightpink"] = CreateScope<ColorHex>("ffb6c1");
        colors["lightsalmon"] = CreateScope<ColorHex>("ffa07a");
        colors["lightseagreen"] = CreateScope<ColorHex>("20b2aa");
        colors["lightskyblue"] = CreateScope<ColorHex>("87cefa");
        colors["lightslategray"] = CreateScope<ColorHex>("778899");
        colors["lightslategrey"] = CreateScope<ColorHex>("778899");
        colors["lightsteelblue"] = CreateScope<ColorHex>("b0c4de");
        colors["lightyellow"] = CreateScope<ColorHex>("ffffe0");
        colors["lime"] = CreateScope<ColorHex>("00ff00");
        colors["limegreen"] = CreateScope<ColorHex>("32cd32");
        colors["linen"] = CreateScope<ColorHex>("faf0e6");
        colors["magenta"] = CreateScope<ColorHex>("ff00ff");
        colors["maroon"] = CreateScope<ColorHex>("800000");
        colors["maroon2"] = CreateScope<ColorHex>("7f0000");
        colors["maroon3"] = CreateScope<ColorHex>("b03060");
        colors["mediumaquamarine"] = CreateScope<ColorHex>("66cdaa");
        colors["mediumblue"] = CreateScope<ColorHex>("0000cd");
        colors["mediumorchid"] = CreateScope<ColorHex>("ba55d3");
        colors["mediumpurple"] = CreateScope<ColorHex>("9370db");
        colors["mediumseagreen"] = CreateScope<ColorHex>("3cb371");
        colors["mediumslateblue"] = CreateScope<ColorHex>("7b68ee");
        colors["mediumspringgreen"] = CreateScope<ColorHex>("00fa9a");
        colors["mediumturquoise"] = CreateScope<ColorHex>("48d1cc");
        colors["mediumvioletred"] = CreateScope<ColorHex>("c71585");
        colors["midnightblue"] = CreateScope<ColorHex>("191970");
        colors["mintcream"] = CreateScope<ColorHex>("f5fffa");
        colors["mistyrose"] = CreateScope<ColorHex>("ffe4e1");
        colors["moccasin"] = CreateScope<ColorHex>("ffe4b5");
        colors["navajowhite"] = CreateScope<ColorHex>("ffdead");
        colors["navy"] = CreateScope<ColorHex>("000080");
        colors["oldlace"] = CreateScope<ColorHex>("fdf5e6");
        colors["olive"] = CreateScope<ColorHex>("808000");
        colors["olivedrab"] = CreateScope<ColorHex>("6b8e23");
        colors["orange"] = CreateScope<ColorHex>("ffa500");
        colors["orangered"] = CreateScope<ColorHex>("ff4500");
        colors["orchid"] = CreateScope<ColorHex>("da70d6");
        colors["palegoldenrod"] = CreateScope<ColorHex>("eee8aa");
        colors["palegreen"] = CreateScope<ColorHex>("98fb98");
        colors["paleturquoise"] = CreateScope<ColorHex>("afeeee");
        colors["palevioletred"] = CreateScope<ColorHex>("db7093");
        colors["papayawhip"] = CreateScope<ColorHex>("ffefd5");
        colors["peachpuff"] = CreateScope<ColorHex>("ffdab9");
        colors["peru"] = CreateScope<ColorHex>("cd853f");
        colors["pink"] = CreateScope<ColorHex>("ffc0cb");
        colors["plum"] = CreateScope<ColorHex>("dda0dd");
        colors["powderblue"] = CreateScope<ColorHex>("b0e0e6");
        colors["purple"] = CreateScope<ColorHex>("800080");
        colors["purple2"] = CreateScope<ColorHex>("7f007f");
        colors["purple3"] = CreateScope<ColorHex>("a020f0");
        colors["rebeccapurple"] = CreateScope<ColorHex>("663399");
        colors["red"] = CreateScope<ColorHex>("ff0000");
        colors["rosybrown"] = CreateScope<ColorHex>("bc8f8f");
        colors["royalblue"] = CreateScope<ColorHex>("4169e1");
        colors["saddlebrown"] = CreateScope<ColorHex>("8b4513");
        colors["salmon"] = CreateScope<ColorHex>("fa8072");
        colors["sandybrown"] = CreateScope<ColorHex>("f4a460");
        colors["seagreen"] = CreateScope<ColorHex>("2e8b57");
        colors["seashell"] = CreateScope<ColorHex>("fff5ee");
        colors["sienna"] = CreateScope<ColorHex>("a0522d");
        colors["silver"] = CreateScope<ColorHex>("c0c0c0");
        colors["skyblue"] = CreateScope<ColorHex>("87ceeb");
        colors["slateblue"] = CreateScope<ColorHex>("6a5acd");
        colors["slategray"] = CreateScope<ColorHex>("708090");
        colors["slategrey"] = CreateScope<ColorHex>("708090");
        colors["snow"] = CreateScope<ColorHex>("fffafa");
        colors["springgreen"] = CreateScope<ColorHex>("00ff7f");
        colors["steelblue"] = CreateScope<ColorHex>("4682b4");
        colors["tan"] = CreateScope<ColorHex>("d2b48c");
        colors["teal"] = CreateScope<ColorHex>("008080");
        colors["thistle"] = CreateScope<ColorHex>("d8bfd8");
        colors["tomato"] = CreateScope<ColorHex>("ff6347");
        colors["turquoise"] = CreateScope<ColorHex>("40e0d0");
        colors["violet"] = CreateScope<ColorHex>("ee82ee");
        colors["wheat"] = CreateScope<ColorHex>("f5deb3");
        colors["white"] = CreateScope<ColorHex>("ffffff");
        colors["whitesmoke"] = CreateScope<ColorHex>("f5f5f5");
        colors["yellow"] = CreateScope<ColorHex>("ffff00");
        colors["yellowgreen"] = CreateScope<ColorHex>("9acd32");
    }
}
