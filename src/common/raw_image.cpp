#include "raw_image.hpp"

void RawImage::decode(const Format& format, InputStream& in)
{
    format.get_decoder()->decode(in, *this);
}

void RawImage::encode(const Format& format, OutputStream& out)
{
    format.get_encoder()->encode(out, *this);
}
