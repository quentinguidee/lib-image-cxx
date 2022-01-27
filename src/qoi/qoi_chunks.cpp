#include "qoi_chunks.hpp"

#include "pixel.hpp"
#include "qoi_decode_exception.hpp"

uint8_t QOI::hash_pixel(const Pixel& pixel)
{
    return (pixel.r * 3 + pixel.g * 5 + pixel.b * 7 + pixel.a * 11) % 64;
}

/*
 * HEADER
 */

QOI::Header::Header(InputStream& in)
{
    decode(in);
}

QOI::Header::Header(uint32_t width, uint32_t height, Channels channels, Colorspace colorspace) :
    width(width), height(height), channels(channels), colorspace(colorspace)
{
}

void QOI::Header::encode(OutputStream& out)
{
    out.write_u32(MAGIC);
    out.write_u32(width);
    out.write_u32(height);
    out.write_u8(channels == Channels::RGB ? 3 : 4);
    out.write_u8(colorspace == Colorspace::SRGB ? 0 : 1);
}

void QOI::Header::decode(InputStream& in)
{
    in.read_u32();
    width = in.read_u32();
    height = in.read_u32();
    channels = in.read_u8() == 3 ? Channels::RGB : Channels::RGBA;
    colorspace = in.read_u8() == 0 ? Colorspace::SRGB : Colorspace::LINEAR;
}

bool QOI::Header::operator==(const Header& rhs) const
{
    return width == rhs.width &&
           height == rhs.height &&
           channels == rhs.channels &&
           colorspace == rhs.colorspace;
}

/*
 * INDEX
 */

QOI::Index::Index(InputStream& in)
{
    decode(in);
}

QOI::Index::Index(uint8_t index) :
    index(index)
{
}

void QOI::Index::encode(OutputStream& out)
{
    out.write_u8(TAG | index);
}

void QOI::Index::decode(InputStream& in)
{
    index = in.read_u8();
}

bool QOI::Index::operator==(const Index& rhs) const
{
    return index == rhs.index;
}

/*
 * DIFF
 */

QOI::Diff::Diff(InputStream& in)
{
    decode(in);
}

QOI::Diff::Diff(int8_t diff_red, int8_t diff_green, int8_t diff_blue) :
    diff_red(diff_red + 2), diff_green(diff_green + 2), diff_blue(diff_blue + 2)
{
}

void QOI::Diff::encode(OutputStream& out)
{
    uint8_t value = diff_red << 4 | diff_green << 2 | diff_blue;
    out.write_u8(TAG | (value & 0x3f));
}

void QOI::Diff::decode(InputStream& in)
{
    uint8_t value = in.read_u8();
    diff_red = (value & 0x30) >> 4;
    diff_green = (value & 0x0c) >> 2;
    diff_blue = value & 0x03;
}

void QOI::Diff::apply_to(Pixel& pixel) const
{
    pixel.r += diff_red - 2;
    pixel.g += diff_green - 2;
    pixel.b += diff_blue - 2;
}

bool QOI::Diff::operator==(const Diff& rhs) const
{
    return diff_red == rhs.diff_red &&
           diff_green == rhs.diff_green &&
           diff_blue == rhs.diff_blue;
}

/*
 * RGB
 */

QOI::RGB::RGB(InputStream& in)
{
    decode(in);
}

QOI::RGB::RGB(const Pixel& pixel) :
    pixel(pixel)
{
}

void QOI::RGB::encode(OutputStream& out)
{
    out.write_u8(TAG);
    out.write_u8(pixel.r);
    out.write_u8(pixel.g);
    out.write_u8(pixel.b);
}

void QOI::RGB::decode(InputStream& in)
{
    in.read_u8();
    pixel.r = in.read_u8();
    pixel.g = in.read_u8();
    pixel.b = in.read_u8();
}

bool QOI::RGB::operator==(const RGB& rhs) const
{
    return pixel.r == rhs.pixel.r &&
           pixel.g == rhs.pixel.g &&
           pixel.b == rhs.pixel.b;
}

/*
 * RGBA
 */

QOI::RGBA::RGBA(InputStream& in)
{
    decode(in);
}

QOI::RGBA::RGBA(const Pixel& pixel) :
    pixel(pixel)
{
}

void QOI::RGBA::encode(OutputStream& out)
{
    out.write_u8(TAG);
    out.write_u8(pixel.r);
    out.write_u8(pixel.g);
    out.write_u8(pixel.b);
    out.write_u8(pixel.a);
}

void QOI::RGBA::decode(InputStream& in)
{
    in.read_u8();
    pixel.r = in.read_u8();
    pixel.g = in.read_u8();
    pixel.b = in.read_u8();
    pixel.a = in.read_u8();
}

bool QOI::RGBA::operator==(const RGBA& rhs) const
{
    return pixel.r == rhs.pixel.r &&
           pixel.g == rhs.pixel.g &&
           pixel.b == rhs.pixel.b &&
           pixel.a == rhs.pixel.a;
}

/*
 * LUMA
 */

QOI::Luma::Luma(InputStream& in)
{
    decode(in);
}

QOI::Luma::Luma(int8_t diff_green, int8_t diff_red_green, int8_t diff_blue_green) :
    diff_green(diff_green + 32), diff_red_green(diff_red_green + 8), diff_blue_green(diff_blue_green + 8)
{
}

void QOI::Luma::encode(OutputStream& out)
{
    out.write_u8(TAG | diff_green);
    out.write_u8(diff_red_green << 4 | diff_blue_green);
}

void QOI::Luma::decode(InputStream& in)
{
    uint8_t value = in.read_u8();
    diff_green = value & 0x3f;

    value = in.read_u8();
    diff_red_green = (value & 0xf0) >> 4;
    diff_blue_green = value & 0x0f;
}

void QOI::Luma::apply_to(Pixel& pixel) const
{
    pixel.r += (diff_green - 32) + (diff_red_green - 8);
    pixel.g += (diff_green - 32);
    pixel.b += (diff_green - 32) + (diff_blue_green - 8);
}

bool QOI::Luma::operator==(const Luma& rhs) const
{
    return diff_green == rhs.diff_green &&
           diff_red_green == rhs.diff_red_green &&
           diff_blue_green == rhs.diff_blue_green;
}

/*
 * Run
 */

QOI::Run::Run(InputStream& in)
{
    decode(in);
}

QOI::Run::Run(uint8_t run) :
    run((run - 1) & 0x3f)
{
}

void QOI::Run::encode(OutputStream& out)
{
    out.write_u8(TAG | run);
}

void QOI::Run::decode(InputStream& in)
{
    run = in.read_u8() & 0x3f;
}

bool QOI::Run::operator==(const Run& rhs) const
{
    return run == rhs.run;
}

/*
 * Run
 */

QOI::Footer::Footer(InputStream& in)
{
    decode(in);
}

void QOI::Footer::encode(OutputStream& out)
{
    out.write_u32(0x00000000);
    out.write_u32(0x00000001);
}

void QOI::Footer::decode([[maybe_unused]] InputStream& in)
{
    // Nothing to do.
    // The footer is always the same.
}
