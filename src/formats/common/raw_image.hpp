#pragma once

#include "decoder.hpp"
#include "encoder.hpp"
#include "pixel.hpp"
#include "stream.hpp"

struct RawImage
{
    Pixels pixels;
    uint32_t width = 0, height = 0;

    RawImage() {}

    void decode(const BaseDecoder& decoder, InputStream& in);
    void encode(const BaseEncoder& encoder, OutputStream& out);
};
