#include <cstdio>
#include <fstream>
#include <utility>

#include "gtest/gtest.h"
#include "stream.hpp"

TEST(CommonTest, BufferStream)
{
    BufferStream stream;

    stream.write_u8(0);
    stream.write_u8(12);
    stream.write_u8(255);
    stream.write_i8(-4);
    stream.write_i8(127);
    stream.write_u16(30000);
    stream.write_i16(-25000);
    stream.write_u32(4000000000);
    stream.write_i32(2000000000);
    stream.write_i32(-2000000000);

    EXPECT_EQ(stream.size(), 21);

    EXPECT_EQ(stream.peek_u8(), 0);
    EXPECT_EQ(stream.read_u8(), 0);
    EXPECT_EQ(stream.read_u8(), 12);
    EXPECT_EQ(stream.peek_u8(), 255);
    EXPECT_EQ(stream.read_u8(), 255);
    EXPECT_EQ(stream.peek_i8(), -4);
    EXPECT_EQ(stream.read_i8(), -4);
    EXPECT_EQ(stream.peek_i8(), 127);
    EXPECT_EQ(stream.read_i8(), 127);
    EXPECT_EQ(stream.read_u16(), 30000);
    EXPECT_EQ(stream.read_i16(), -25000);
    EXPECT_EQ(stream.read_u32(), 4000000000);
    EXPECT_EQ(stream.read_i32(), 2000000000);
    EXPECT_EQ(stream.read_i32(), -2000000000);
}

TEST(CommonTest, FileStream)
{
    OutputFileStream out("temp");

    ASSERT_TRUE(out.is_open());

    out.write_u8(0);
    out.write_u8(12);
    out.write_u8(255);
    out.write_i8(-4);
    out.write_i8(127);
    out.write_u16(30000);
    out.write_i16(-25000);
    out.write_u32(4000000000);
    out.write_i32(2000000000);
    out.write_i32(-2000000000);

    out.close();

    InputFileStream in("temp");

    ASSERT_TRUE(in.is_open());

    EXPECT_EQ(in.size(), 21);

    EXPECT_EQ(in.peek_u8(), 0);
    EXPECT_EQ(in.read_u8(), 0);
    EXPECT_EQ(in.read_u8(), 12);
    EXPECT_EQ(in.peek_u8(), 255);
    EXPECT_EQ(in.read_u8(), 255);
    EXPECT_EQ(in.peek_i8(), -4);
    EXPECT_EQ(in.read_i8(), -4);
    EXPECT_EQ(in.peek_i8(), 127);
    EXPECT_EQ(in.read_i8(), 127);
    EXPECT_EQ(in.read_u16(), 30000);
    EXPECT_EQ(in.read_i16(), -25000);
    EXPECT_EQ(in.read_u32(), 4000000000);
    EXPECT_EQ(in.read_i32(), 2000000000);
    EXPECT_EQ(in.read_i32(), -2000000000);

    in.close();

    remove("temp");
}
