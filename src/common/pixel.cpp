#include "pixel.hpp"

bool Pixel::operator==(const Pixel& rhs) const
{
    return r == rhs.r &&
           g == rhs.g &&
           b == rhs.b &&
           a == rhs.a;
}
