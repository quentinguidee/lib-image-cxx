#pragma once

#include <deque>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

class InputStream
{
public:
    virtual uint8_t read_u8() = 0;
    virtual int8_t read_i8() = 0;
    virtual uint8_t peek_u8() = 0;
    virtual int8_t peek_i8() = 0;

    // Read 2 bytes as an unsigned integer (big endian)
    uint16_t read_u16();
    // Read 2 bytes as an unsigned integer (little endian)
    uint16_t read_u16_le();

    // Read 2 bytes as a signed integer (big endian)
    int16_t read_i16();
    // Read 2 bytes as a signed integer (little endian)
    int16_t read_i16_le();

    // Read 3 bytes as an unsigned integer (big endian)
    uint32_t read_u24();
    // Read 3 bytes as an unsigned integer (little endian)
    uint32_t read_u24_le();

    // Read 3 bytes as a signed integer (big endian)
    int32_t read_i24();
    // Read 3 bytes as a signed integer (little endian)
    int32_t read_i24_le();

    // Read 4 bytes as an unsigned integer (big endian)
    uint32_t read_u32();
    // Read 4 bytes as an unsigned integer (little endian)
    uint32_t read_u32_le();

    // Read 4 bytes as a signed integer (big endian)
    int32_t read_i32();
    // Read 4 bytes as a signed integer (little endian)
    int32_t read_i32_le();

    virtual long size() = 0;
};

class OutputStream
{
public:
    virtual void write_u8(uint8_t value) = 0;
    virtual void write_i8(int8_t value) = 0;

    // Write an unsigned integer of 2 bytes (big endian)
    void write_u16(uint16_t value);
    // Write an unsigned integer of 2 bytes (little endian)
    void write_u16_le(uint16_t value);

    // Write a signed integer of 2 bytes (big endian)
    void write_i16(int16_t value);
    // Write a signed integer of 2 bytes (little endian)
    void write_i16_le(int16_t value);

    // Write an unsigned integer of 3 bytes (big endian)
    void write_u24(uint32_t value);
    // Write an unsigned integer of 3 bytes (little endian)
    void write_u24_le(uint32_t value);

    // Write a signed integer of 3 bytes (big endian)
    void write_i24(int32_t value);
    // Write a signed integer of 3 bytes (little endian)
    void write_i24_le(int32_t value);

    // Write an unsigned integer of 4 bytes (big endian)
    void write_u32(uint32_t value);
    // Write an unsigned integer of 4 bytes (little endian)
    void write_u32_le(uint32_t value);

    // Write a signed integer of 4 bytes (big endian)
    void write_i32(int32_t value);
    // Write a signed integer of 4 bytes (little endian)
    void write_i32_le(int32_t value);
};

class BufferStream : public InputStream, public OutputStream
{
private:
    std::deque<uint8_t> queue;

public:
    uint8_t read_u8() override;
    int8_t read_i8() override;
    uint8_t peek_u8() override { return queue.front(); }
    int8_t peek_i8() override { return queue.front(); }

    void write_u8(uint8_t value) override;
    void write_i8(int8_t value) override;

    void clear() { queue.clear(); }
    std::string as_string() const;

    long size() override { return queue.size(); }
};

class InputFileStream : public InputStream
{
private:
    std::ifstream input;

public:
    InputFileStream(const std::string& filename);

    uint8_t read_u8() override { return input.get(); }
    int8_t read_i8() override { return input.get(); }
    uint8_t peek_u8() override { return input.peek(); }
    int8_t peek_i8() override { return input.peek(); }

    void close() { input.close(); }
    bool is_open() const { return input.is_open(); }

    long size() override;
};

class OutputFileStream : public OutputStream
{
private:
    std::ofstream output;

public:
    OutputFileStream(const std::string& filename);

    void write_u8(uint8_t value) override;
    void write_i8(int8_t value) override;

    void close() { output.close(); }
    bool is_open() const { return output.is_open(); }
};
