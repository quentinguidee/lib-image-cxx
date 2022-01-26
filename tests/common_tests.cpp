#include <cstdio>
#include <fstream>
#include <utility>

#include "gtest/gtest.h"
#include "stream.hpp"

TEST(CommonTest, BufferStream)
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

TEST(CommonTest, FileStream)
{
    OutputFileStream out("temp");

    EXPECT_TRUE(out.is_open());

    out.write_8(0);
    out.write_8(12);
    out.write_8(42);

    out.close();

    InputFileStream in("temp");

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
