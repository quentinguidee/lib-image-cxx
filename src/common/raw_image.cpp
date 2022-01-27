#include "raw_image.hpp"

void RawImage::decode(const BaseDecoder& decoder, InputStream& in)
{
    decoder.decode(in, *this);
}

void RawImage::encode(const BaseEncoder& encoder, OutputStream& out)
{
    encoder.encode(out, *this);
}
