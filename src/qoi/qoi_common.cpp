#include "qoi_common.hpp"

namespace QOI {

uint8_t hash_pixel(const Pixel& pixel)
{
    return (pixel.r * 3 + pixel.g * 5 + pixel.b * 7 + pixel.a * 11) % 64;
}

}
