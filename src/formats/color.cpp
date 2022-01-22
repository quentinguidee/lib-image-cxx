#include "color.hpp"

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
    red(red), green(green), blue(blue), alpha(alpha)
{
}

uint8_t Color::hash() const
{
    return (red * 3 + green * 5 + blue * 7 + alpha * 11) % 64;
}

bool Color::operator==(const Color& rhs) const
{
    return red == rhs.red &&
           green == rhs.green &&
           blue == rhs.blue &&
           alpha == rhs.alpha;
}
