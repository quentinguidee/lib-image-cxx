#include "deflate_compressor.hpp"

#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>

#include "bits.hpp"
#include "stream.hpp"

namespace Compression {

const std::vector<DeflateTables::ExtraLength> DeflateTables::extra_lengths = {
    { 257, 0, 3 },
    { 258, 0, 4 },
    { 259, 0, 5 },
    { 260, 0, 6 },
    { 261, 0, 7 },
    { 262, 0, 8 },
    { 263, 0, 9 },
    { 264, 0, 10 },
    { 265, 1, 11 },
    { 266, 1, 13 },
    { 267, 1, 15 },
    { 268, 1, 17 },
    { 269, 2, 19 },
    { 270, 2, 23 },
    { 271, 2, 27 },
    { 272, 2, 31 },
    { 273, 3, 35 },
    { 274, 3, 43 },
    { 275, 3, 51 },
    { 276, 3, 59 },
    { 277, 4, 67 },
    { 278, 4, 83 },
    { 279, 4, 99 },
    { 280, 4, 115 },
    { 281, 5, 131 },
    { 282, 5, 163 },
    { 283, 5, 195 },
    { 284, 5, 227 },
    { 285, 0, 258 },
};

const std::vector<DeflateTables::ExtraDistance> DeflateTables::extra_distances = {
    { 0, 0, 1 },
    { 1, 0, 2 },
    { 2, 0, 3 },
    { 3, 0, 4 },
    { 4, 1, 5 },
    { 5, 1, 7 },
    { 6, 2, 9 },
    { 7, 2, 13 },
    { 8, 3, 17 },
    { 9, 3, 25 },
    { 10, 4, 33 },
    { 11, 4, 49 },
    { 12, 5, 65 },
    { 13, 5, 97 },
    { 14, 6, 129 },
    { 15, 6, 193 },
    { 16, 7, 257 },
    { 17, 7, 385 },
    { 18, 8, 513 },
    { 19, 8, 769 },
    { 20, 9, 1025 },
    { 21, 9, 1537 },
    { 22, 10, 2049 },
    { 23, 10, 3073 },
    { 24, 11, 4097 },
    { 25, 11, 6145 },
    { 26, 12, 8193 },
    { 27, 12, 12289 },
    { 28, 13, 16385 },
    { 29, 13, 24577 },
};

DeflateTables::ExtraLength DeflateTables::get_extra_length(uint16_t code)
{
    for (auto it = extra_lengths.begin(); it != extra_lengths.end(); ++it)
        if (it->code == code) return *it;

    throw std::runtime_error { "Code not found in get_extra_length()." };
}

DeflateTables::ExtraDistance DeflateTables::get_extra_distance(uint16_t code)
{
    for (auto it = extra_distances.begin(); it != extra_distances.end(); ++it)
        if (it->code == code) return *it;

    throw std::runtime_error { "Code not found in get_extra_distance()." };
}

HuffmanNode::HuffmanNode(int length, int symbol) :
    length { length },
    symbol { symbol }
{
}

HuffmanTree::HuffmanTree(std::vector<std::shared_ptr<HuffmanNode>> nodes)
{
    // Build code_to_length + length_count dictionnary
    int max_length = 0;
    std::map<uint32_t, uint32_t> code_to_length;
    std::map<uint32_t, uint32_t> length_count;
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        code_to_length[(*it)->symbol] = (*it)->length;
        if ((*it)->length > max_length) max_length = (*it)->length;
        ++length_count[(*it)->length];
    }

    // Build next_code dictionnary
    length_count[0] = 0;
    std::map<uint32_t, uint32_t> next_code;
    uint32_t code = 0, max_code = 0;
    for (int length = 1; length <= max_length; ++length)
    {
        code = (code + length_count[length - 1]) << 1;
        if (code > max_code) max_code = code;
        next_code[length] = code;
    }

    // Build final table
    for (uint32_t i = 0; i < nodes.size(); ++i)
    {
        int length = nodes[i]->length;
        if (length != 0)
        {
            nodes[i]->code = next_code[length];
            next_code[length]++;
        }
    }

    for (auto it = nodes.begin(); it != nodes.end(); ++it)
        code_to_symbol[(*it)->code] = (*it)->symbol;
}

int HuffmanTree::get_value(InputStream& in)
{
    if (in.remaining_size() == 0)
        throw std::runtime_error { "HuffmanTree::get_value() cannot run with an empty InputStream." };

    uint32_t read_value = 0;
    std::map<int, int>::iterator it;
    do
    {
        read_value = (read_value << 1) | in.read_u1_le();
        it = code_to_symbol.find(read_value);
    } while (it == code_to_symbol.end());
    return it->second;
}

DeflateCompressor::DeflateCompressor(InputStream& in) :
    in { in }
{
}

std::vector<uint8_t> DeflateCompressor::uncompress()
{
    out.clear();

    const uint8_t method = in.read_bits_le<uint8_t>(4);
    const int8_t info = in.read_bits_le<uint8_t>(4);
    if (method != 8)
        throw std::runtime_error { "Cannot deflate with a method of " + std::to_string(method) + "." };

    in.read_bits_le<uint8_t>(5);
    uint8_t fdict = in.read_bits_le<uint8_t>(1);
    uint8_t flevel = in.read_bits_le<uint8_t>(2);

    bool is_last_block = false;
    do
    {
        is_last_block = in.read_bits_le<uint8_t>(1);
        uint8_t compression_level = in.read_bits_le<uint8_t>(2);
        read_block(compression_level);

        // TODO: Check ALDER32
        in.skip(4);
    } while (!is_last_block);

    return out;
}

void DeflateCompressor::read_block(uint8_t compression_level)
{
    switch (compression_level)
    {
        case 0:
            read_block_compression_level_0();
            break;
        case 1:
            read_block_compression_level_1();
            break;
        case 2:
            read_block_compression_level_2();
            break;
        default:
            throw std::runtime_error { "The compression level " + std::to_string(compression_level) + " cannot be decompressed." };
    }
}

void DeflateCompressor::read_block_compression_level_0()
{
    std::cout << "LEVEL 0" << std::endl;

    uint16_t length = in.read_u16_le();
    uint16_t length_inverted = in.read_u16_le();

    if (length != (uint16_t)~length_inverted)
        throw std::runtime_error { std::to_string(length) + " and " + std::to_string(~length_inverted) + " are not equals. Stream integrity error." };

    for (int i = 0; i < length; ++i)
        out.push_back(in.read_u8());
}

void DeflateCompressor::read_block_compression_level_1()
{
    std::cout << "LEVEL 1" << std::endl;

    std::vector<std::shared_ptr<HuffmanNode>> length_nodes {};
    std::vector<std::shared_ptr<HuffmanNode>> distance_nodes {};

    length_nodes.reserve(288);
    distance_nodes.reserve(32);

    for (int i = 0; i <= 287; ++i)
        length_nodes.push_back(std::make_shared<HuffmanNode>(size_of_code(i), i));

    for (int i = 0; i <= 31; ++i)
        distance_nodes.push_back(std::make_shared<HuffmanNode>(5, i));

    HuffmanTree length_tree { length_nodes };
    HuffmanTree distance_tree { distance_nodes };

    BufferStream stream;
    stream.write_u8(0b11111101);
    std::cout << length_tree.get_value(stream) << std::endl;
    // SHOULD BE 143

    throw std::exception();

    bool loop = true;
    while (loop)
    {
        std::cout << "========== (" << in.remaining_size() << ") ==========" << std::endl;

        int length_code = length_tree.get_value(in);

        std::cout << "CODE           | " << length_code << std::endl;

        if (length_code == 256)
            loop = false;
        else if (length_code <= 255)
            out.push_back((uint8_t)length_code);
        else
        {
            std::cout << "REPEAT" << std::endl;
            DeflateTables::ExtraLength length = DeflateTables::get_extra_length(length_code);
            uint8_t length_additional_bits = length.bits;
            int length_value = length.length << length_additional_bits;
            length_value += in.read_bits_le<int>(length_additional_bits);

            std::cout << "LENGTH: " << length_value << std::endl;

            uint16_t distance_code = distance_tree.get_value(in);

            DeflateTables::ExtraDistance distance = DeflateTables::get_extra_distance(distance_code);
            uint8_t distance_additional_bits = distance.bits;
            int distance_value = distance.distance << distance_additional_bits;
            distance_value += in.read_bits_le<int>(distance_additional_bits);

            std::cout << "DISTANCE: " << distance_value << std::endl;

            auto start = out.end() - distance_value;
            auto end = start + length_value;
            if (end > out.end()) end = out.end();
            std::vector<char> repeated { start, end };

            std::string s { repeated.begin(), repeated.end() };
            std::cout << "REPEATED: " << s << std::endl;

            // TODO: MISSING REPETITION
            out.insert(out.end(), repeated.begin(), repeated.end());
        }
        std::string s { out.begin(), out.end() };
        std::cout << "OUTPUT         | " << s << std::endl;
    }
}

void DeflateCompressor::read_block_compression_level_2()
{
    std::cout << "LEVEL 2" << std::endl;
}

uint8_t DeflateCompressor::size_of_code(uint16_t code) const
{
    if (code < 0)
        throw std::runtime_error { "DeflateCompressor::size_of_code(): Code cannot be negative (" + std::to_string(code) + ")" };

    if (code <= 143)
        return 8;
    if (code <= 255)
        return 9;
    if (code <= 279)
        return 7;
    if (code <= 287)
        return 8;

    throw std::runtime_error { std::to_string(code) + " is not a valid code." };
}
}
