#include <cstdio>
#include <fstream>
#include <utility>

#include "gtest/gtest.h"
#include "qoi_decoder.hpp"
#include "qoi_encoder.hpp"
#include "qoi_format.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

TEST(QOITest, QOIConversion)
{
    InputFileStream in("../test_images/image_1.qoi");

    ASSERT_TRUE(in.is_open());

    RawImage image = RawImage();
    image.decode(QOI_FORMAT, in);

    EXPECT_EQ(image.width, 1920);
    EXPECT_EQ(image.height, 1139);
    EXPECT_EQ(image.pixels.size(), image.width * image.height);
    EXPECT_EQ(image.channels, Channels::RGBA);

    OutputFileStream out("../test_images/image_1_out.qoi");

    ASSERT_TRUE(out.is_open());

    image.encode(QOI_FORMAT, out);

    out.close();

    std::ifstream original("../test_images/image_1.qoi");
    std::ifstream exported("../test_images/image_1_out.qoi");
    while (!original.eof() && !exported.eof())
        ASSERT_EQ(original.get(), exported.get());

    std::remove("../test_images/image_1_out.qoi");
}
