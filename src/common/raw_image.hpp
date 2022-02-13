#pragma once

#include <string>

#include "decoder.hpp"
#include "encoder.hpp"
#include "pixel.hpp"
#include "stream.hpp"

struct RawImage
{
    Pixels pixels;

    uint32_t width = 0, height = 0;

    RawImage() {}

    const Pixel &get_pixel(uint32_t x, uint32_t y);

    void flip_vertically();
};
