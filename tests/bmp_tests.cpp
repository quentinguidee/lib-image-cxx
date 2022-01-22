#include <cstdio>
#include <fstream>
#include <utility>

#include "formats/bmp/chunks.hpp"
#include "formats/bmp/image.hpp"
#include "formats/color.hpp"
#include "formats/stream.hpp"
#include "gtest/gtest.h"

TEST(BMPFormatTest, Header)
{
    BufferStream stream;
    std::pair<BMP::Header, std::string> headers[] = {
        {{32, 10}, "424d00000020000000000000000a"},
        {{10, 20}, "424d0000000a0000000000000014"},
    };

    for (auto header : headers)
    {
        // Encoding
        header.first.encode(stream);
        EXPECT_EQ(stream.as_string(), header.second);

        // Decoding
        BMP::Header new_header(stream);
        EXPECT_EQ(new_header, header.first);
    }
}
