#include <exception>

#include "bmp_decoder.hpp"
#include "bmp_encoder.hpp"
#include "exceptions.hpp"
#include "gtest/gtest.h"
#include "raw_image.hpp"
#include "stream.hpp"

#define TEST_BMP_ENCODING_DECODING(NAMESPACE, FILENAME, NAME) \
    TEST(NAMESPACE##Test, NAMESPACE##Conversion##NAME)        \
    {                                                         \
        convert_bmp_to_bmp(FILENAME);                         \
    }

void convert_bmp_to_bmp(const std::string& filename)
{
    std::string input_path = "../test_images/" + filename;
    std::string output_path = "../test_images/out_" + filename;

    InputFileStream in(input_path);

    ASSERT_TRUE(in.is_open());

    EXPECT_TRUE(BMP::Decoder::can_decode(in));

    RawImage image = RawImage();
    BMP::Decoder decoder(in, image);
    decoder.decode();
    BMP::Settings settings = decoder.get_settings();

    OutputFileStream out(output_path);

    ASSERT_TRUE(out.is_open());

    BMP::Encoder encoder(out, image);
    encoder.set_settings(settings);
    encoder.encode();

    out.close();

    std::ifstream original(input_path);
    std::ifstream exported(output_path);
    for (int count = 0; !original.eof() && !exported.eof(); ++count)
        ASSERT_EQ(original.get(), exported.get()) << "Different byte at position " << count;

    std::remove(output_path.c_str());
}

TEST_BMP_ENCODING_DECODING(BMP, "image_1.bmp", Default);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_2bpp_x8r8g8b8.bmp", 2bppX8R8G8B8);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_4bpp_x8r8g8b8.bmp", 4bppX8R8G8B8);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_8bpp_x8r8g8b8.bmp", 8bppX8R8G8B8);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_16bpp_r5g6b5.bmp", 16bppR5G6B5);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_16bpp_a1r5g5b5.bmp", 16bppA1R5G5B5);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_16bpp_x1r5g5b5.bmp", 16bppX1R5G5B5);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_24bpp_r8g8b8.bmp", 24bppR8G8B8);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_32bpp_a8r8g8b8.bmp", 32bppA8R8G8B8);
TEST_BMP_ENCODING_DECODING(BMP, "image_1_small_32bpp_x8r8g8b8.bmp", 32bppX8R8G8B8);
