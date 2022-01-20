#pragma once

#include <vector>

#include "chunks.hpp"
#include "stream.hpp"

namespace QOI {

class Image
{
private:
    std::vector<Color> pixels;
    uint32_t width, height;
    Channels channels;
    Colorspace colorspace;

public:
    void encode(OutputStream& out);
    void decode(InputStream& in);

    // void show();

    const std::vector<Color>& get_pixels() const { return pixels; }
    uint32_t get_width() const { return width; }
    uint32_t get_height() const { return height; }
    Channels get_channels() const { return channels; }
};

}
