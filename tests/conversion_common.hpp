#pragma once

#include "gtest/gtest.h"
#include "raw_image.hpp"
#include "stream.hpp"

#define TEST_ENCODE_AND_DECODE(NAMESPACE, FILENAME, NAME)                                         \
    TEST(NAMESPACE##Test, NAMESPACE##EncodeAndDecode##NAME)                                       \
    {                                                                                             \
        encode_and_decode<NAMESPACE::Decoder, NAMESPACE::Encoder, NAMESPACE::Settings>(FILENAME); \
    }

template <typename Decoder, typename Encoder, typename Settings>
void encode_and_decode(const std::string& filename)
{
    std::string input_path { "../test_images/" + filename };
    std::string output_path { "../test_images/out_" + filename };

    InputFileStream in { input_path };

    ASSERT_TRUE(in.is_open());

    EXPECT_TRUE(Decoder::can_decode(in));

    RawImage image;
    Decoder decoder { in, image };
    decoder.decode();
    Settings settings = decoder.get_settings();

    OutputFileStream out { output_path };

    ASSERT_TRUE(out.is_open());

    Encoder encoder { out, image };
    encoder.set_settings(settings);
    encoder.encode();

    out.close();

    std::ifstream original { input_path };
    std::ifstream exported { output_path };
    for (int count = 0; !original.eof() && !exported.eof(); ++count)
        ASSERT_EQ(original.get(), exported.get()) << "Different byte at position " << count;

    std::remove(output_path.c_str());
}
