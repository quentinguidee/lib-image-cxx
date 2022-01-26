#pragma once

#include "pixel.hpp"
#include "stream.hpp"

class RawImage;

class BaseEncoder
{
public:
    virtual void encode(OutputStream& out, RawImage& image) const = 0;
};
