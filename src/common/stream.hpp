#pragma once

#include <cstdint>
#include <fstream>
#include <istream>
#include <sstream>

#include "bits.hpp"

class InputStream
{
private:
    uint8_t current_bit = 0;

public:
    uint8_t peek_u1() { return (get_input_stream().peek() & (0b10000000 >> current_bit)) >> (7 - current_bit); }
    uint8_t peek_u1_le() { return (get_input_stream().peek() >> current_bit) & 1; }
    uint8_t read_u1() { return read_bits(1); }
    uint8_t read_u1_le() { return read_bits_le(1); }

    uint8_t peek_u8();
    uint8_t read_u8();
    int8_t peek_i8();
    int8_t read_i8();

    uint16_t peek_u16() { return peek<uint16_t>(2); }
    uint16_t peek_u16_le() { return peek_le<uint16_t>(2); }
    uint16_t read_u16() { return read<uint16_t>(2); }
    uint16_t read_u16_le() { return read_le<uint16_t>(2); }
    int16_t peek_i16() { return peek<int16_t>(2); }
    int16_t peek_i16_le() { return peek_le<int16_t>(2); }
    int16_t read_i16() { return read<int16_t>(2); }
    int16_t read_i16_le() { return read_le<int16_t>(2); }

    uint32_t peek_u24() { return peek<uint32_t>(3); }
    uint32_t peek_u24_le() { return peek_le<uint32_t>(3); }
    uint32_t read_u24() { return read<uint32_t>(3); }
    uint32_t read_u24_le() { return read_le<uint32_t>(3); }
    int32_t peek_i24() { return peek<int32_t>(3); }
    int32_t peek_i24_le() { return peek_le<int32_t>(3); }
    int32_t read_i24() { return read<int32_t>(3); }
    int32_t read_i24_le() { return read_le<int32_t>(3); }

    uint32_t peek_u32() { return peek<uint32_t>(4); }
    uint32_t peek_u32_le() { return peek_le<uint32_t>(4); }
    uint32_t read_u32() { return read<uint32_t>(4); }
    uint32_t read_u32_le() { return read_le<uint32_t>(4); }
    int32_t peek_i32() { return peek<int32_t>(4); }
    int32_t peek_i32_le() { return peek_le<int32_t>(4); }
    int32_t read_i32() { return read<int32_t>(4); }
    int32_t read_i32_le() { return read_le<int32_t>(4); }

    void go_back(uint8_t n_bytes = 1);
    void skip(uint8_t n_bytes = 1);
    void goto_complete_byte();

    long size();
    long remaining_size();

    virtual std::istream& get_input_stream() = 0;

    template <typename T>
    T peek(uint8_t n_bytes)
    {
        T value = read<T>(n_bytes);
        go_back(n_bytes);
        return value;
    }

    template <typename T>
    T peek_le(uint8_t n_bytes)
    {
        T value = read_le<T>(n_bytes);
        go_back(n_bytes);
        return value;
    }

    template <typename T>
    T read(uint8_t n_bytes)
    {
        goto_complete_byte();
        T value = 0;
        for (int16_t offset = (n_bytes - 1) * BYTE; offset >= 0; offset -= BYTE)
            value |= (T)read_u8() << offset;
        return value;
    }

    template <typename T>
    T read_le(uint8_t n_bytes)
    {
        goto_complete_byte();
        T value = 0;
        for (uint16_t offset = 0; offset <= (n_bytes - 1) * BYTE; offset += BYTE)
            value |= (T)read_u8() << offset;
        return value;
    }

    uint8_t read_bits(uint8_t n_bits);
    uint8_t read_bits_le(uint8_t n_bits);
};

class OutputStream
{
public:
    void write_u8(uint8_t value) { get_output_stream() << value; };
    void write_i8(int8_t value) { get_output_stream() << value; };

    void write_u16(uint16_t value) { write<uint16_t>(2, value); }
    void write_u16_le(uint16_t value) { write_le<uint16_t>(2, value); }
    void write_i16(int16_t value) { write<int16_t>(2, value); }
    void write_i16_le(int16_t value) { write_le<int16_t>(2, value); }

    void write_u24(uint32_t value) { write<uint32_t>(3, value); }
    void write_u24_le(uint32_t value) { write_le<uint32_t>(3, value); }
    void write_i24(int32_t value) { write<int32_t>(3, value); }
    void write_i24_le(int32_t value) { write_le<int32_t>(3, value); }

    void write_u32(uint32_t value) { write<uint32_t>(4, value); }
    void write_u32_le(uint32_t value) { write_le<uint32_t>(4, value); }
    void write_i32(int32_t value) { write<int32_t>(4, value); }
    void write_i32_le(int32_t value) { write_le<int32_t>(4, value); }

    virtual std::ostream& get_output_stream() = 0;

    template <typename T>
    void write(uint8_t n_bytes, T value)
    {
        for (int16_t offset = (n_bytes - 1) * BYTE; offset >= 0; offset -= BYTE)
            write_u8(value >> offset);
    }

    template <typename T>
    void write_le(uint8_t n_bytes, T value)
    {
        for (uint16_t offset = 0; offset <= (n_bytes - 1) * BYTE; offset += BYTE)
            write_u8(value >> offset);
    }
};

class BufferStream : public InputStream, public OutputStream
{
private:
    std::stringstream stream;

public:
    void clear() { stream.clear(); }

    std::istream& get_input_stream() override { return stream; }
    std::ostream& get_output_stream() override { return stream; }
};

class InputFileStream : public InputStream
{
private:
    std::ifstream input;

public:
    InputFileStream(const std::string& filename);

    void close() { input.close(); }
    bool is_open() const { return input.is_open(); }

    std::istream& get_input_stream() override { return input; }
};

class OutputFileStream : public OutputStream
{
private:
    std::ofstream output;

public:
    OutputFileStream(const std::string& filename);

    void close() { output.close(); }
    bool is_open() const { return output.is_open(); }

    std::ostream& get_output_stream() override { return output; }
};
