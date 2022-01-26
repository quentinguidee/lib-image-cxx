#pragma once

#include <vector>

#include "qoi_chunks.hpp"
#include "stream.hpp"

namespace QOI {

class Image
{
private:
    std::vector<Color> pixels;
    Header header;

public:
    void encode(OutputStream& out);
    void decode(InputStream& in);

    // void show();

    const std::vector<Color>& get_pixels() const { return pixels; }
    uint32_t get_width() const { return header.get_width(); }
    uint32_t get_height() const { return header.get_height(); }
    Channels get_channels() const { return header.get_channels(); }
    Colorspace get_colorspace() const { return header.get_colorspace(); }
};

}
