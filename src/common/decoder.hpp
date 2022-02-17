#pragma once

#include "pixel.hpp"
#include "stream.hpp"

struct RawImage;

class BaseDecoder
{
protected:
    InputStream& in;
    RawImage& image;

public:
    BaseDecoder(InputStream& in, RawImage& image) :
        in { in }, image { image } {}

    virtual ~BaseDecoder() = default;

    virtual void decode() = 0;
};
