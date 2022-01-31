#pragma once

#include "pixel.hpp"
#include "stream.hpp"

struct RawImage;

class BaseDecoder
{
public:
    virtual ~BaseDecoder() = default;

    virtual void decode(InputStream& in, RawImage& image) const = 0;
};
