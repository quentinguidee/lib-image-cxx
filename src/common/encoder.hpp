#pragma once

#include "pixel.hpp"
#include "stream.hpp"

struct RawImage;

class BaseEncoder
{
public:
    virtual ~BaseEncoder() = default;

    virtual void encode(OutputStream& out, RawImage& image) const = 0;
};
