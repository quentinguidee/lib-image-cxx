#pragma once

#include <cstdint>
#include <vector>

struct Pixel
{
    uint8_t r, g, b, a;

    bool operator==(const Pixel& rhs) const;
};

typedef std::vector<Pixel> Pixels;
