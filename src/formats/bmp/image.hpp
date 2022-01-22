#pragma once

#include <vector>

#include "../color.hpp"
#include "../stream.hpp"

namespace BMP {

class Image
{
private:
    std::vector<Color> pixels;

public:
    void encode(OutputStream& out);
    void decode(InputStream& in);

    const std::vector<Color>& get_pixels() const { return pixels; }
};

}
