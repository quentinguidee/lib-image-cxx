#include "raw_image.hpp"

#include <algorithm>

#include "decoder.hpp"

const Pixel& RawImage::get_pixel(uint32_t x, uint32_t y)
{
    return pixels[y * width + x];
}

void RawImage::flip_vertically()
{
    for (uint32_t i = 0; i < height / 2; ++i)
        for (uint32_t j = 0; j < width; ++j)
            std::swap(pixels[(i * width) + j], pixels[((height - i) * width) + j]);
}
