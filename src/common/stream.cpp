#include "stream.hpp"

#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>

#include "bits.hpp"

uint8_t InputStream::read_u1()
{
    uint8_t value = peek_u1();
    goto_next_bit();
    return value;
}

uint8_t InputStream::read_u1_le()
{
    uint8_t value = peek_u1_le();
    goto_next_bit();
    return value;
}

uint8_t InputStream::peek_u8()
{
    goto_complete_byte();
    return get_input_stream().peek();
}

uint8_t InputStream::read_u8()
{
    goto_complete_byte();
    return get_input_stream().get();
}

int8_t InputStream::peek_i8()
{
    goto_complete_byte();
    return get_input_stream().peek();
}

int8_t InputStream::read_i8()
{
    goto_complete_byte();
    return get_input_stream().get();
}

void InputStream::go_back(uint8_t n_bytes)
{
    current_bit = 0;
    get_input_stream().seekg(-n_bytes, std::ios::cur);
}

void InputStream::goto_next_bit()
{
    if (++current_bit == 8)
        goto_complete_byte();
}

void InputStream::skip(uint8_t n_bytes)
{
    current_bit = 0;
    get_input_stream().seekg(n_bytes, std::ios::cur);
}

void InputStream::goto_complete_byte()
{
    if (current_bit == 0) return;
    skip(1);
}

void InputStream::goto_start()
{
    current_bit = 0;
    get_input_stream().seekg(0, std::ios::beg);
}

void InputStream::goto_end()
{
    current_bit = 0;
    get_input_stream().seekg(0, std::ios::end);
}

long InputStream::size()
{
    // TODO: Handle tellg error (if it returns -1).

    int current_bit = this->current_bit;

    std::istream& input_stream = get_input_stream();

    long current_position = input_stream.tellg();

    goto_start();
    long begin = input_stream.tellg();
    goto_end();
    long end = input_stream.tellg();
    input_stream.seekg(current_position);

    this->current_bit = current_bit;

    return end - begin;
}

long InputStream::remaining_size()
{
    // TODO: Handle tellg error (if it returns -1).

    int current_bit = this->current_bit;

    std::istream& input_stream = get_input_stream();

    long current_position = input_stream.tellg();
    goto_end();
    long end = input_stream.tellg();
    input_stream.seekg(current_position);

    this->current_bit = current_bit;

    return end - current_position;
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
