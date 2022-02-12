#pragma once

#include <deque>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

class InputStream
{
public:
    virtual uint8_t peek_u8() = 0;
    virtual int8_t peek_i8() = 0;

    virtual uint8_t read_u8() = 0;
    virtual int8_t read_i8() = 0;

    uint16_t read_u16() { return read(2); }
    uint16_t read_u16_le() { return read_le(2); }
    int16_t read_i16() { return read(2); }
    int16_t read_i16_le() { return read_le(2); }

    uint32_t read_u24() { return read(3); }
    uint32_t read_u24_le() { return read_le(3); }
    int32_t read_i24() { return read(3); }
    int32_t read_i24_le() { return read_le(3); }

    uint32_t read_u32() { return read(4); }
    uint32_t read_u32_le() { return read_le(4); }
    int32_t read_i32() { return read(4); }
    int32_t read_i32_le() { return read_le(4); }

    virtual long size() = 0;

private:
    uint32_t read(uint8_t n_bytes);
    uint32_t read_le(uint8_t n_bytes);
};

class OutputStream
{
public:
    virtual void write_u8(uint8_t value) = 0;
    virtual void write_i8(int8_t value) = 0;

    void write_u16(uint16_t value) { write(2, value); }
    void write_u16_le(uint16_t value) { write_le(2, value); }
    void write_i16(int16_t value) { write(2, value); }
    void write_i16_le(int16_t value) { write_le(2, value); }

    void write_u24(uint32_t value) { write(3, value); }
    void write_u24_le(uint32_t value) { write_le(3, value); }
    void write_i24(int32_t value) { write(3, value); }
    void write_i24_le(int32_t value) { write_le(3, value); }

    void write_u32(uint32_t value) { write(4, value); }
    void write_u32_le(uint32_t value) { write_le(4, value); }
    void write_i32(int32_t value) { write(4, value); }
    void write_i32_le(int32_t value) { write_le(4, value); }

private:
    void write(uint8_t n_bytes, uint32_t value);
    void write_le(uint8_t n_bytes, uint32_t value);
};

class BufferStream : public InputStream, public OutputStream
{
private:
    std::deque<uint8_t> queue;

public:
    uint8_t peek_u8() override { return queue.front(); }
    int8_t peek_i8() override { return queue.front(); }

    uint8_t read_u8() override;
    int8_t read_i8() override;

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
