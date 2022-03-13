#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "deflate_compressor.hpp"
#include "gtest/gtest.h"
#include "stream.hpp"

static void test_write(OutputStream& out)
{
    out.write_u8(0);
    out.write_u8(12);
    out.write_u8(255);
    out.write_i8(-4);
    out.write_u8(0b10100110);
    out.write_i8(127);
    out.write_u8(0b10100110);
    out.write_u16(30000);
    out.write_i16(-25000);
    out.write_u16(0x1234);
    out.write_u16_le(0x1234);
    out.write_u32(4000000000);
    out.write_i32(2000000000);
    out.write_i32(-2000000000);
    out.write_i24(0xABCDEF);
    out.write_u32(0x12345678);
    out.write_u32_le(0x12345678);
    out.write<uint64_t>(8, 0xFEDCBA9876543210);
}
static void test_read(InputStream& in)
{
    EXPECT_EQ(in.size(), 46);

    EXPECT_EQ(in.peek_u8(), 0);
    EXPECT_EQ(in.read_u8(), 0);
    EXPECT_EQ(in.read_u8(), 12);
    EXPECT_EQ(in.peek_u8(), 255);
    EXPECT_EQ(in.read_u8(), 255);
    EXPECT_EQ(in.peek_i8(), -4);
    EXPECT_EQ(in.read_i8(), -4);
    EXPECT_EQ(in.read_u1_le(), 0);
    EXPECT_EQ(in.read_u1_le(), 1);
    EXPECT_EQ(in.peek_u1_le(), 1);
    EXPECT_EQ(in.read_u1_le(), 1);
    EXPECT_EQ(in.read_u1_le(), 0);
    EXPECT_EQ(in.peek_i8(), 127);
    EXPECT_EQ(in.read_i8(), 127);
    EXPECT_EQ(in.read_u1(), 1);
    EXPECT_EQ(in.read_u1(), 0);
    EXPECT_EQ(in.peek_u1(), 1);
    EXPECT_EQ(in.read_u1(), 1);
    EXPECT_EQ(in.read_bits<uint8_t>(4), 0b0011);
    EXPECT_EQ(in.peek_u16(), 30000);
    EXPECT_EQ(in.read_u16(), 30000);
    EXPECT_EQ(in.peek_i16(), -25000);
    EXPECT_EQ(in.read_i16(), -25000);
    EXPECT_EQ(in.peek_u16(), 0x1234);
    EXPECT_EQ(in.read_u16(), 0x1234);
    EXPECT_EQ(in.peek_u16(), 0x3412);
    EXPECT_EQ(in.read_u16(), 0x3412);
    in.skip(2);
    in.go_back(2);
    EXPECT_EQ(in.peek_u32(), 4000000000);
    EXPECT_EQ(in.read_u32(), 4000000000);
    EXPECT_EQ(in.peek_i32(), 2000000000);
    EXPECT_EQ(in.read_i32(), 2000000000);
    EXPECT_EQ(in.peek_i32(), -2000000000);
    EXPECT_EQ(in.read_i32(), -2000000000);
    in.skip(3);
    EXPECT_EQ(in.peek_i32(), 0x12345678);
    EXPECT_EQ(in.read_i32(), 0x12345678);
    EXPECT_EQ(in.peek_u32(), 0x78563412);
    EXPECT_EQ(in.read_u32(), 0x78563412);
    EXPECT_EQ(in.read<uint64_t>(8), 0xFEDCBA9876543210);
}

TEST(CommonTest, BufferStream)
{
    BufferStream stream;
    test_write(stream);
    test_read(stream);
}

TEST(CommonTest, FileStream)
{
    OutputFileStream out { "temporary_stream_test" };
    ASSERT_TRUE(out.is_open());
    test_write(out);
    out.close();

    InputFileStream in { "temporary_stream_test" };
    ASSERT_TRUE(in.is_open());
    test_read(in);
    in.close();

    remove("temporary_stream_test");
}

TEST(CommonTest, HuffmanTree)
{
    std::vector<std::shared_ptr<Compression::HuffmanNode>> nodes = {
        std::make_shared<Compression::HuffmanNode>(2, 'A'),
        std::make_shared<Compression::HuffmanNode>(1, 'B'),
        std::make_shared<Compression::HuffmanNode>(3, 'C'),
        std::make_shared<Compression::HuffmanNode>(3, 'D'),
    };

    Compression::HuffmanTree tree { nodes };
    BufferStream stream {};
    stream.write_u16_le(0b0000001101110101);
    ASSERT_EQ(tree.get_value(stream), 'A');
    ASSERT_EQ(tree.get_value(stream), 'A');
    ASSERT_EQ(tree.get_value(stream), 'D');
    ASSERT_EQ(tree.get_value(stream), 'B');
    ASSERT_EQ(tree.get_value(stream), 'C');
}

TEST(CommonTest, DeflateCompressorLevel0)
{
    BufferStream stream {};

    stream.write_u32(0x7801010e);
    stream.write_u32(0x00f1ff53);
    stream.write_u32(0x6f6d6520);
    stream.write_u32(0x74657874);
    stream.write_u32(0x20686572);
    stream.write_u32(0x6526e205);
    stream.write_u8(0x3e);

    Compression::DeflateCompressor compressor { stream };
    std::vector<uint8_t> uncompressed_data = compressor.uncompress();
    std::string s { uncompressed_data.begin(), uncompressed_data.end() };
    EXPECT_EQ(s, "Some text here");
}

TEST(CommonTest, DeflateCompressorLevel1)
{
    BufferStream stream {};

    stream.write_u32(0x78da0bce);
    stream.write_u32(0xcf4d5528);
    stream.write_u32(0x49ad2851);
    stream.write_u32(0xc8482d02);
    stream.write_u32(0xb232124b);
    stream.write_u32(0x14328b15);
    stream.write_u32(0x8a520b52);
    stream.write_u32(0x134b5253);
    stream.write_u32(0x74148af1);
    stream.write_u32(0x2bd00300);
    stream.write_u32(0x09bc1783);

    Compression::DeflateCompressor compressor { stream };
    std::vector<uint8_t> uncompressed_data = compressor.uncompress();
    std::string s { uncompressed_data.begin(), uncompressed_data.end() };
    EXPECT_EQ(s, "Some text here that is repeated, some text here that is repeated.");
}
