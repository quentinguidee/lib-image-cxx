#include "stream.hpp"

#include <fstream>
#include <ios>
#include <istream>
#include <ostream>
#include <sstream>

#include "bits.hpp"

void InputStream::go_back(uint8_t n_bytes)
{
    get_input_stream().seekg(-n_bytes, std::ios::cur);
}

void InputStream::skip(uint8_t n_bytes)
{
    get_input_stream().seekg(n_bytes, std::ios::cur);
}

void InputStream::goto_complete_byte()
{
    if (current_bit == 0) return;
    skip(1);
    current_bit = 0;
}

long InputStream::size()
{
    // TODO: Handle tellg error (if it returns -1).

    std::istream& input_stream = get_input_stream();

    long current_position = input_stream.tellg();

    input_stream.seekg(0, std::ios::beg);
    long begin = input_stream.tellg();
    input_stream.seekg(0, std::ios::end);
    long end = input_stream.tellg();
    input_stream.seekg(current_position);

    return end - begin;
}

long InputStream::remaining_size()
{
    // TODO: Handle tellg error (if it returns -1).

    std::istream& input_stream = get_input_stream();

    long current_position = input_stream.tellg();
    input_stream.seekg(0, std::ios::end);
    long end = input_stream.tellg();
    input_stream.seekg(current_position);

    return end - current_position;
}

uint8_t InputStream::read_bits(uint8_t n_bits)
{
    assert(n_bits <= 8);

    uint8_t value = 0;
    for (uint8_t i = 0; i < n_bits; ++i)
    {
        value = (value << 1) + peek_u1();
        if (++current_bit == 8)
            goto_complete_byte();
    }
    return value;
}

InputFileStream::InputFileStream(const std::string& filename) :
    input { std::ifstream { filename } }
{
    input.seekg(0, std::ios::beg);
}

OutputFileStream::OutputFileStream(const std::string& filename) :
    output { std::ofstream { filename } }
{
}
