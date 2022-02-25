#include <cstdio>
#include <fstream>
#include <utility>

#include "gtest/gtest.h"
#include "stream.hpp"

static void test_write(OutputStream& out)
{
    out.write_u8(0);
    out.write_u8(12);
    out.write_u8(255);
    out.write_i8(-4);
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
    EXPECT_EQ(in.size(), 45);

    EXPECT_EQ(in.peek_u8(), 0);
    EXPECT_EQ(in.read_u8(), 0);
    EXPECT_EQ(in.read_u8(), 12);
    EXPECT_EQ(in.peek_u8(), 255);
    EXPECT_EQ(in.read_u8(), 255);
    EXPECT_EQ(in.peek_i8(), -4);
    EXPECT_EQ(in.read_i8(), -4);
    EXPECT_EQ(in.peek_i8(), 127);
    EXPECT_EQ(in.read_i8(), 127);
    EXPECT_EQ(in.read_u1(), 1);
    EXPECT_EQ(in.read_u1(), 0);
    EXPECT_EQ(in.peek_u1(), 1);
    EXPECT_EQ(in.read_u1(), 1);
    EXPECT_EQ(in.read_bits(4), 0b0011);
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
