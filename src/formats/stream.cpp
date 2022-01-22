#include "stream.hpp"

#include <fstream>
#include <iomanip>
#include <ios>
#include <ostream>
#include <sstream>

uint16_t InputStream::read_16()
{
    uint16_t value;
    value |= read_8() << 8;
    value |= read_8();
    return value;
}

uint32_t InputStream::read_32()
{
    uint32_t value = 0;
    value |= read_8() << 24;
    value |= read_8() << 16;
    value |= read_8() << 8;
    value |= read_8();
    return value;
}

void OutputStream::write_16(uint16_t value)
{
    write_8((value & 0xff00) >> 8);
    write_8(value & 0x00ff);
}

void OutputStream::write_32(uint32_t value)
{
    write_8((value & 0xff000000) >> 24);
    write_8((value & 0x00ff0000) >> 16);
    write_8((value & 0x0000ff00) >> 8);
    write_8(value & 0x000000ff);
}

void BufferStream::write_8(uint8_t value)
{
    queue.push_back(value);
}

uint8_t BufferStream::read_8()
{
    uint8_t value = queue.front();
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

void OutputFileStream::write_8(uint8_t value)
{
    output << value;
}

OutputFileStream::OutputFileStream(const std::string& filename) :
    output(std::ofstream(filename))
{
}
