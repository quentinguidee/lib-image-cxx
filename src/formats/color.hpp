#pragma once

#include <cstdint>

struct Color
{
    uint8_t red, green, blue, alpha;

    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255);

    uint8_t hash() const;

    bool operator==(const Color& rhs) const;
};
