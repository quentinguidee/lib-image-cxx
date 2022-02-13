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

    EXPECT_TRUE(QOI::Decoder::can_decode(in));

    RawImage image = RawImage();
    QOI::Decoder decoder(in, image);
    decoder.decode();
    QOI::Settings settings = decoder.get_settings();

    OutputFileStream out("../test_images/image_1_out.qoi");

    ASSERT_TRUE(out.is_open());

    QOI::Encoder encoder(out, image);
    encoder.set_settings(settings);
    encoder.encode();

    out.close();

    std::ifstream original("../test_images/image_1.qoi");
    std::ifstream exported("../test_images/image_1_out.qoi");
    while (!original.eof() && !exported.eof())
        ASSERT_EQ(original.get(), exported.get());

    std::remove("../test_images/image_1_out.qoi");
}
