#include "bmp_decoder.hpp"
#include "bmp_encoder.hpp"
#include "conversion_common.hpp"
#include "gtest/gtest.h"
#include "qoi_decoder.hpp"
#include "qoi_encoder.hpp"

TEST_ENCODE_AND_DECODE(QOI, "image_1.qoi", Default);

TEST_ENCODE_AND_DECODE(BMP, "image_1.bmp", Default);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_2bpp_x8r8g8b8.bmp", 2bppX8R8G8B8);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_4bpp_x8r8g8b8.bmp", 4bppX8R8G8B8);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_8bpp_x8r8g8b8.bmp", 8bppX8R8G8B8);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_16bpp_r5g6b5.bmp", 16bppR5G6B5);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_16bpp_a1r5g5b5.bmp", 16bppA1R5G5B5);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_16bpp_x1r5g5b5.bmp", 16bppX1R5G5B5);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_24bpp_r8g8b8.bmp", 24bppR8G8B8);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_32bpp_a8r8g8b8.bmp", 32bppA8R8G8B8);
TEST_ENCODE_AND_DECODE(BMP, "image_1_small_32bpp_x8r8g8b8.bmp", 32bppX8R8G8B8);
