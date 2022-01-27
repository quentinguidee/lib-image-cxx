#pragma once

#include "pixel.hpp"
#include "stream.hpp"

struct RawImage;

class BaseDecoder
{
public:
    virtual void decode(InputStream& in, RawImage& image) const = 0;
};
