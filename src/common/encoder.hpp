#pragma once

#include "pixel.hpp"
#include "stream.hpp"

struct RawImage;

class BaseEncoder
{
protected:
    OutputStream& out;
    RawImage& image;

public:
    BaseEncoder(OutputStream& out, RawImage& image) :
        out { out }, image { image } {}

    virtual ~BaseEncoder() = default;

    virtual void encode() = 0;
};
