#pragma once

#include <string>

#include "channels.hpp"
#include "colorspace.hpp"
#include "decoder.hpp"
#include "encoder.hpp"
#include "format.hpp"
#include "pixel.hpp"
#include "stream.hpp"

struct RawImage
{
    Pixels pixels;

    uint32_t width = 0, height = 0;
    Colorspace colorspace;
    Channels channels;

    std::string filename;
    Format format;

    RawImage() {}

    void decode(const Format& format, InputStream& in);
    void encode(const Format& format, OutputStream& out);
};
