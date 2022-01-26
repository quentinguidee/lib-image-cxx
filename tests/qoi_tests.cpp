#include <cstdio>
#include <fstream>
#include <utility>

#include "gtest/gtest.h"
#include "qoi_chunks.hpp"
#include "qoi_image.hpp"
#include "stream.hpp"

TEST(QOIFormatTest, Header)
{
    BufferStream stream;
    std::pair<QOI::Header, std::string> headers[] = {
        {{32, 32, QOI::Channels::RGBA, QOI::Colorspace::SRGB}, "716f696600000020000000200400"},
        {{1, 4, QOI::Channels::RGB, QOI::Colorspace::LINEAR}, "716f696600000001000000040301"},
        {{1400, 6000, QOI::Channels::RGBA, QOI::Colorspace::SRGB}, "716f696600000578000017700400"},
    };

    for (auto header : headers)
    {
        // Encoding
        header.first.encode(stream);
        EXPECT_EQ(stream.as_string(), header.second);

        // Decoding
        QOI::Header new_header(stream);
        EXPECT_EQ(new_header, header.first);
    }
}

TEST(QOIFormatTest, Index)
{
    BufferStream stream;
    std::pair<QOI::Index, std::string> indexes[] = {
        {{17}, "11"},
        {{0}, "00"},
        {{1}, "01"},
        {{63}, "3f"},
    };

    for (auto index : indexes)
    {
        // Encoding
        index.first.encode(stream);
        EXPECT_EQ(stream.as_string(), index.second);

        // Decoding
        QOI::Index new_index(stream);
        EXPECT_EQ(new_index, index.first);
    }
}

TEST(QOIFormatTest, Diff)
{
    BufferStream stream;
    std::pair<QOI::Diff, std::string> diffs[] = {
        {{0, -1, 0}, "66"},
        {{-2, -2, -2}, "40"},
        {{-1, -1, -1}, "55"},
        {{1, 1, 1}, "7f"},
    };

    for (auto diff : diffs)
    {
        // Encoding
        diff.first.encode(stream);
        EXPECT_EQ(stream.as_string(), diff.second);

        // Decoding
        QOI::Diff new_diff(stream);
        EXPECT_EQ(new_diff, diff.first);
    }
}

TEST(QOIFormatTest, RGB)
{
    BufferStream stream;
    std::pair<QOI::RGB, std::string> pixels[] = {
        {{{0, 0, 0}}, "fe000000"},
        {{{30, 40, 50}}, "fe1e2832"},
    };

    for (auto pixel : pixels)
    {
        // Encoding
        pixel.first.encode(stream);
        EXPECT_EQ(stream.as_string(), pixel.second);

        // Decoding
        QOI::RGB new_rgb(stream);
        EXPECT_EQ(new_rgb, pixel.first);
    }
}

TEST(QOIFormatTest, RGBA)
{
    BufferStream stream;
    std::pair<QOI::RGBA, std::string> pixels[] = {
        {{{0, 0, 0, 0}}, "ff00000000"},
        {{{255, 255, 255, 255}}, "ffffffffff"},
    };

    for (auto pixel : pixels)
    {
        // Encoding
        pixel.first.encode(stream);
        EXPECT_EQ(stream.as_string(), pixel.second);

        // Decoding
        QOI::RGBA new_rgba(stream);
        EXPECT_EQ(new_rgba, pixel.first);
    }
}

TEST(QOIFormatTest, Luma)
{
    BufferStream stream;
    std::pair<QOI::Luma, std::string> lumas[] = {
        {{-32, -8, -8}, "8000"},
        {{-12, 2, 2}, "94aa"},
        {{31, 7, 7}, "bfff"},
    };

    for (auto luma : lumas)
    {
        // Encoding
        luma.first.encode(stream);
        EXPECT_EQ(stream.as_string(), luma.second);

        // Decoding
        QOI::Luma new_luma(stream);
        EXPECT_EQ(new_luma, luma.first);
    }
}

TEST(QOIFormatTest, Run)
{
    BufferStream stream;
    std::pair<QOI::Run, std::string> runs[] = {
        {{1}, "c0"},
        {{21}, "d4"},
        {{64}, "ff"},
    };

    for (auto run : runs)
    {
        // Encoding
        run.first.encode(stream);
        EXPECT_EQ(stream.as_string(), run.second);

        // Decoding
        QOI::Run new_run(stream);
        EXPECT_EQ(new_run, run.first);
    }
}

TEST(QOIFormatTest, Footer)
{
    BufferStream stream;
    QOI::Footer footer;

    // Encoding
    footer.encode(stream);
    EXPECT_EQ(stream.as_string(), "0000000000000001");

    // Decoding
    QOI::Footer new_footer(stream);
    EXPECT_EQ(new_footer, QOI::Footer());
}

TEST(QOIFormatTest, BufferStream)
{
    BufferStream stream;

    stream.write_8(0);
    stream.write_8(12);
    stream.write_8(42);

    EXPECT_EQ(stream.size(), 3);

    EXPECT_EQ(stream.peek_8(), 0);
    EXPECT_EQ(stream.read_8(), 0);
    EXPECT_EQ(stream.read_8(), 12);
    EXPECT_EQ(stream.peek_8(), 42);
    EXPECT_EQ(stream.read_8(), 42);
}

TEST(QOIFormatTest, FileStream)
{
    OutputFileStream out("temp.qoi");

    EXPECT_TRUE(out.is_open());

    out.write_8(0);
    out.write_8(12);
    out.write_8(42);

    out.close();

    InputFileStream in("temp.qoi");

    EXPECT_TRUE(in.is_open());

    EXPECT_EQ(in.size(), 3);

    EXPECT_EQ(in.peek_8(), 0);
    EXPECT_EQ(in.read_8(), 0);
    EXPECT_EQ(in.read_8(), 12);
    EXPECT_EQ(in.peek_8(), 42);
    EXPECT_EQ(in.read_8(), 42);

    in.close();

    remove("temp.qoi");
}

TEST(QOIFormatTest, Color)
{
    QOI::Color color{255, 1, 99, 255};

    color.red += 1;
    color.green -= 2;
    color.blue += 101;

    EXPECT_EQ(color.red, 0);
    EXPECT_EQ(color.green, 255);
    EXPECT_EQ(color.blue, 200);
}

TEST(QOIFormatTest, QOIToImage)
{
    InputFileStream in("test_images/image_1.qoi");

    ASSERT_TRUE(in.is_open());

    QOI::Image image;
    image.decode(in);

    EXPECT_EQ(image.get_width(), 1920);
    EXPECT_EQ(image.get_height(), 1139);
    EXPECT_EQ(image.get_pixels().size(), image.get_width() * image.get_height());
    EXPECT_EQ(image.get_channels(), QOI::Channels::RGBA);

    OutputFileStream out("test_images/image_1_out.qoi");

    ASSERT_TRUE(out.is_open());

    image.encode(out);

    out.close();

    std::ifstream original("test_images/image_1.qoi");
    std::ifstream exported("test_images/image_1_out.qoi");
    while (!original.eof() && !exported.eof())
        ASSERT_EQ(original.get(), exported.get());
}
