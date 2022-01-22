#include "chunks.hpp"

/*
 * HEADER
 */

BMP::Header::Header(InputStream& in)
{
    decode(in);
}

BMP::Header::Header(uint32_t size_in_bytes, uint32_t offset) :
    size_in_bytes(size_in_bytes), offset(offset)
{
}

void BMP::Header::encode(OutputStream& out)
{
    out.write_16(TAG);
    out.write_32(size_in_bytes);
    out.write_16(0x0000);
    out.write_16(0x0000);
    out.write_32(offset);
}

void BMP::Header::decode(InputStream& in)
{
    in.read_16();  // 'BM'
    size_in_bytes = in.read_32();
    in.read_16();  // Reserved
    in.read_16();  // Reserved
    offset = in.read_32();
}

bool BMP::Header::operator==(const Header& rhs) const
{
    return size_in_bytes == rhs.size_in_bytes &&
           offset == rhs.offset;
}
