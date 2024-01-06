#pragma once

#include "gp.h"
#include "ColorHex.h"
#include <unordered_map>

namespace gp {
    GPAPI extern std::unordered_map<std::string, Scope<ColorHex>> colors;

    GPAPI void initW3CX11();
}
