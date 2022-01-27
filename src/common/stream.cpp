#include "stream.hpp"

#include <fstream>
#include <iomanip>
#include <ios>
#include <ostream>
#include <sstream>

uint16_t InputStream::read_u16()
{
    uint16_t value = 0;
    value |= read_u8() << 8;
    value |= read_u8();
    return value;
}

int16_t InputStream::read_i16()
{
    int16_t value = 0;
    value |= read_u8() << 8;
    value |= read_u8();
    return value;
}

uint32_t InputStream::read_u32()
{
    uint32_t value = 0;
    value |= read_u8() << 24;
    value |= read_u8() << 16;
    value |= read_u8() << 8;
    value |= read_u8();
    return value;
}

int32_t InputStream::read_i32()
{
    int32_t value = 0;
    value |= read_u8() << 24;
    value |= read_u8() << 16;
    value |= read_u8() << 8;
    value |= read_u8();
    return value;
}

void OutputStream::write_u16(uint16_t value)
{
    write_u8((value & 0xff00) >> 8);
    write_u8(value & 0x00ff);
}

void OutputStream::write_i16(int16_t value)
{
    write_i8((value & 0xff00) >> 8);
    write_i8(value & 0x00ff);
}

void OutputStream::write_u32(uint32_t value)
{
    write_u8((value & 0xff000000) >> 24);
    write_u8((value & 0x00ff0000) >> 16);
    write_u8((value & 0x0000ff00) >> 8);
    write_u8(value & 0x000000ff);
}

void OutputStream::write_i32(int32_t value)
{
    write_i8((value & 0xff000000) >> 24);
    write_i8((value & 0x00ff0000) >> 16);
    write_i8((value & 0x0000ff00) >> 8);
    write_i8(value & 0x000000ff);
}

void BufferStream::write_u8(uint8_t value)
{
    queue.push_back(value);
}

void BufferStream::write_i8(int8_t value)
{
    queue.push_back(value);
}

uint8_t BufferStream::read_u8()
{
    uint8_t value = queue.front();
    queue.pop_front();
    return value;
}

int8_t BufferStream::read_i8()
{
    int8_t value = queue.front();
    queue.pop_front();
    return value;
}

std::string BufferStream::as_string() const
{
    std::stringstream s;
    for (auto it = queue.begin(); it != queue.end(); ++it)
        s << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)*it;
    return s.str();
}

InputFileStream::InputFileStream(const std::string& filename) :
    input(std::ifstream(filename))
{
    input.seekg(0, std::ios::beg);
}

long InputFileStream::size()
{
    // TODO: Handle tellg error (if it returns -1).

    input.seekg(0, std::ios::beg);
    long begin = input.tellg();
    input.seekg(0, std::ios::end);
    long end = input.tellg();
    input.seekg(0, std::ios::beg);

    return end - begin;
}

void OutputFileStream::write_u8(uint8_t value)
{
    output << value;
}

void OutputFileStream::write_i8(int8_t value)
{
    output << value;
}

OutputFileStream::OutputFileStream(const std::string& filename) :
    output(std::ofstream(filename))
{
}
