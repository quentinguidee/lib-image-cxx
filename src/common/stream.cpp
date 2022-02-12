#include "stream.hpp"

#include <fstream>
#include <ios>
#include <ostream>
#include <sstream>

#include "bits.hpp"

long input_stream_size(std::istream& input_stream)
{
    // TODO: Handle tellg error (if it returns -1).

    long current_position = input_stream.tellg();

    input_stream.seekg(0, std::ios::beg);
    long begin = input_stream.tellg();
    input_stream.seekg(0, std::ios::end);
    long end = input_stream.tellg();
    input_stream.seekg(current_position);

    return end - begin;
}

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

InputFileStream::InputFileStream(const std::string& filename) :
    input(std::ifstream(filename))
{
    input.seekg(0, std::ios::beg);
}

OutputFileStream::OutputFileStream(const std::string& filename) :
    output(std::ofstream(filename))
{
}
