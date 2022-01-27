#pragma once

#include <cstdint>
#include <vector>

struct Pixel
{
    uint8_t r, g, b, a;

    Pixel(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

    bool operator==(const Pixel& rhs) const;
};

typedef std::vector<Pixel> Pixels;
