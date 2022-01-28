#include "qoi_common.hpp"

uint8_t QOI::hash_pixel(const Pixel& pixel)
{
    return (pixel.r * 3 + pixel.g * 5 + pixel.b * 7 + pixel.a * 11) % 64;
}
