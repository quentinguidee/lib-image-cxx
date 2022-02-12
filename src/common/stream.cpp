#include "stream.hpp"

#include <fstream>
#include <iomanip>
#include <ios>
#include <ostream>
#include <sstream>

#include "bits.hpp"

uint32_t InputStream::read(uint8_t n_bytes)
{
    uint32_t value = 0;
    for (int8_t offset = (n_bytes - 1) * BYTE; offset >= 0; offset -= BYTE)
        value |= read_u8() << offset;
    return value;
}

uint32_t InputStream::read_le(uint8_t n_bytes)
{
    uint32_t value = 0;
    for (uint8_t offset = 0; offset <= (n_bytes - 1) * BYTE; offset += BYTE)
        value |= read_u8() << offset;
    return value;
}

void OutputStream::write(uint8_t n_bytes, uint32_t value)
{
    for (int8_t offset = (n_bytes - 1) * BYTE; offset >= 0; offset -= BYTE)
        write_u8(value >> offset);
}

void OutputStream::write_le(uint8_t n_bytes, uint32_t value)
{
    for (uint8_t offset = 0; offset <= (n_bytes - 1) * BYTE; offset += BYTE)
        write_u8(value >> offset);
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
