#pragma once

#include "pixel.hpp"
#include "stream.hpp"

class RawImage;

class BaseDecoder
{
public:
    virtual void decode(InputStream& in, RawImage& image) const = 0;
};
