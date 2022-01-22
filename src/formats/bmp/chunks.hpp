#pragma once

#include <cstdint>

#include "../stream.hpp"

namespace BMP {

class Header
{
public:
    static constexpr uint16_t TAG = 0x424d;
    static constexpr uint16_t SIZE = 0x424d;

private:
    // The file size in bytes.
    uint32_t size_in_bytes;

    // The starting address where the pixel array can be found.
    uint32_t offset;

public:
    Header(InputStream& in);
    Header(uint32_t size_in_bytes, uint32_t offset);

    void encode(OutputStream& out);
    void decode(InputStream& in);

    uint32_t get_size_in_bytes() const { return size_in_bytes; }

    bool operator==(const Header& rhs) const;
};

}
