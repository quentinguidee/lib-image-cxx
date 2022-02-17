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

InputFileStream::InputFileStream(const std::string& filename) :
    input { std::ifstream { filename } }
{
    input.seekg(0, std::ios::beg);
}

OutputFileStream::OutputFileStream(const std::string& filename) :
    output { std::ofstream { filename } }
{
}
