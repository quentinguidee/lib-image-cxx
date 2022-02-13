#pragma once

#include <map>
#include <vector>

#include "stream.hpp"

namespace Compression {

struct DeflateTables
{
    struct ExtraLength
    {
        uint16_t code;
        uint8_t bits;
        uint16_t length;
    };

    struct ExtraDistance
    {
        uint16_t code;
        uint8_t bits;
        uint16_t distance;
    };

    static const std::vector<ExtraLength> extra_lengths;
    static const std::vector<ExtraDistance> extra_distances;

    static ExtraLength get_extra_length(uint16_t code);
    static ExtraDistance get_extra_distance(uint16_t code);
};

struct HuffmanNode
{
    int length = 0;
    int symbol = 0;
    int code = 0;

    HuffmanNode(int length = 0, int symbol = 0);
};

class HuffmanTree
{
private:
    std::map<int, int> code_to_symbol = std::map<int, int>();

public:
    HuffmanTree(std::vector<std::shared_ptr<HuffmanNode>> nodes);

    int get_value(InputStream& in);
};

class DeflateCompressor
{
private:
    InputStream& in;
    std::vector<uint8_t> out;

public:
    DeflateCompressor(InputStream& in);

    std::vector<uint8_t> uncompress();

private:
    void read_block(uint8_t compression_level);
    void read_block_compression_level_0();
    void read_block_compression_level_1();
    void read_block_compression_level_2();

    uint8_t size_of_code(uint16_t code) const;
};

}
