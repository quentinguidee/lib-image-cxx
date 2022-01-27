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

    uint16_t read_u16();
    int16_t read_i16();

    uint32_t read_u32();
    int32_t read_i32();

    virtual long size() = 0;
};

class OutputStream
{
public:
    virtual void write_u8(uint8_t value) = 0;
    virtual void write_i8(int8_t value) = 0;

    void write_u16(uint16_t value);
    void write_i16(int16_t value);

    void write_u32(uint32_t value);
    void write_i32(int32_t value);
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
