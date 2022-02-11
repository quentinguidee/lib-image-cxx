#include "qoi_decoder.hpp"

#include "exceptions.hpp"
#include "pixel.hpp"

bool QOI::Decoder::can_decode(InputStream &in)
{
    // TODO: peek_u32 and read 'qoif'
    return in.peek_u8() == 'q';
}

void QOI::Decoder::decode()
{
    decode_header();

    image.pixels.clear();
    image.pixels.reserve(image.width * image.height);

    Pixel previously_seen_pixels[64];
    Pixel pixel;

    uint8_t run = 0;
    long image_size = image.width * image.height;
    for (long i = 0; i < image_size; ++i)
    {
        uint8_t tag_8 = in.peek_u8();
        uint8_t tag_2 = tag_8 & 0xc0;

        if (run > 0)
            --run;
        else if (tag_8 == RGB_TAG)
            decode_rgb(pixel);
        else if (tag_8 == RGBA_TAG)
            decode_rgba(pixel);
        else if (tag_2 == INDEX_TAG)
            pixel = previously_seen_pixels[decode_index()];
        else if (tag_2 == DIFF_TAG)
            decode_diff(pixel);
        else if (tag_2 == LUMA_TAG)
            decode_luma(pixel);
        else if (tag_2 == RUN_TAG)
            run = decode_run();
        else
            throw DecodeException("Tag not recognized.");

        previously_seen_pixels[hash_pixel(pixel)] = pixel;
        image.pixels.push_back(pixel);
    }
}

void QOI::Decoder::decode_header() const
{
    in.read_u32();
    image.width = in.read_u32();
    image.height = in.read_u32();
    image.channels = in.read_u8() == 3 ? Channels::RGB : Channels::RGBA;
    image.colorspace = in.read_u8() == 0 ? Colorspace::SRGB : Colorspace::LINEAR;
}

uint8_t QOI::Decoder::decode_index() const
{
    return in.read_u8();
}

void QOI::Decoder::decode_diff(Pixel &pixel) const
{
    uint8_t value = in.read_u8();

    uint8_t diff_r = (value & 0x30) >> 4;
    uint8_t diff_g = (value & 0x0c) >> 2;
    uint8_t diff_b = value & 0x03;

    pixel.r += diff_r - 2;
    pixel.g += diff_g - 2;
    pixel.b += diff_b - 2;
}

void QOI::Decoder::decode_luma(Pixel &pixel) const
{
    uint8_t value = in.read_u8();
    uint8_t diff_g = value & 0x3f;

    value = in.read_u8();
    uint8_t diff_r_g = (value & 0xf0) >> 4;
    uint8_t diff_b_g = value & 0x0f;

    pixel.r += (diff_g - 32) + (diff_r_g - 8);
    pixel.g += (diff_g - 32);
    pixel.b += (diff_g - 32) + (diff_b_g - 8);
}

void QOI::Decoder::decode_rgb(Pixel &pixel) const
{
    in.read_u8();
    pixel.r = in.read_u8();
    pixel.g = in.read_u8();
    pixel.b = in.read_u8();
}

void QOI::Decoder::decode_rgba(Pixel &pixel) const
{
    in.read_u8();
    pixel.r = in.read_u8();
    pixel.g = in.read_u8();
    pixel.b = in.read_u8();
    pixel.a = in.read_u8();
}

uint8_t QOI::Decoder::decode_run() const
{
    return in.read_u8() & 0x3f;
}
