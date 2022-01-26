#pragma once

#include "channels.hpp"
#include "colorspace.hpp"
#include "decoder.hpp"
#include "encoder.hpp"
#include "pixel.hpp"
#include "stream.hpp"

struct RawImage
{
    Pixels pixels;

    uint32_t width = 0, height = 0;
    Colorspace colorspace;
    Channels channels;

    RawImage() {}

    void decode(const BaseDecoder& decoder, InputStream& in);
    void encode(const BaseEncoder& encoder, OutputStream& out);
};
