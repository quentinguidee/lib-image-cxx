#include "pixel.hpp"

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
    r { r },
    g { g },
    b { b },
    a { a }
{
}

bool Pixel::operator==(const Pixel& rhs) const
{
    return r == rhs.r &&
           g == rhs.g &&
           b == rhs.b &&
           a == rhs.a;
}
