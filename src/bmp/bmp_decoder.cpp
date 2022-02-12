#include "bmp_decoder.hpp"

#include <cmath>
#include <iostream>
#include <string>

#include "bmp_common.hpp"
#include "exceptions.hpp"
#include "log.hpp"
#include "pixel.hpp"

bool BMP::Decoder::can_decode(InputStream& in)
{
    return in.peek_u16() == (uint16_t)Signature::BM;
}

void BMP::Decoder::decode()
{
    decode_header();

    uint32_t header_size = in.read_u32_le();
    settings.header_version = (HeaderVersion)header_size;
    switch (settings.header_version)
    {
        case HeaderVersion::CORE_HEADER:
            decode_core_header();
            break;
        case HeaderVersion::INFO_HEADER_V1:
            decode_info_header_v1();
            break;
        case HeaderVersion::INFO_HEADER_V2:
            decode_info_header_v2();
            break;
        case HeaderVersion::INFO_HEADER_V3:
            decode_info_header_v3();
            break;
        case HeaderVersion::INFO_HEADER_V4:
            decode_info_header_v4();
            break;
        case HeaderVersion::INFO_HEADER_V5:
            decode_info_header_v5();
            break;
        default:
            throw UnsupportedVersionException("Cannot decode headers of size " + std::to_string(header_size) + ".");
    }

    decode_color_table();
    decode_pixel_array();

    if (settings.start_decoding_position == StartDecodingPosition::BottomLeft)
        image.flip_vertically();
}

void BMP::Decoder::decode_header()
{
    settings.signature = (Signature)in.read_u16();
    file_size = in.read_u32_le();
    in.read_u16_le();
    in.read_u16_le();
    settings.starting_address = in.read_u32_le();
}

void BMP::Decoder::decode_core_header()
{
    image.width = in.read_u16_le();
    image.height = in.read_u16_le();
    in.read_u16_le();
    settings.bit_count = (BitCount)in.read_u16_le();
}

void BMP::Decoder::decode_info_header_v1()
{
    image.width = in.read_i32_le();
    int32_t height = in.read_i32_le();
    if (height < 0)
    {
        image.height = -height;
        settings.start_decoding_position = StartDecodingPosition::TopLeft;
    }
    else
    {
        image.height = height;
        settings.start_decoding_position = StartDecodingPosition::BottomLeft;
    }
    in.read_u16_le();
    settings.bit_count = (BitCount)in.read_u16_le();
    settings.compression = (Compression)in.read_u32_le();
    image_size = in.read_u32_le();
    settings.x_pixels_per_meter = in.read_i32_le();
    settings.y_pixels_per_meter = in.read_i32_le();
    settings.colors_count = in.read_u32_le();
    settings.important_colors_count = in.read_u32_le();
}

void BMP::Decoder::decode_info_header_v2()
{
    decode_info_header_v1();
    settings.bitmask_r = Bitmask(in.read_u32_le());
    settings.bitmask_g = Bitmask(in.read_u32_le());
    settings.bitmask_b = Bitmask(in.read_u32_le());
}

void BMP::Decoder::decode_info_header_v3()
{
    decode_info_header_v2();
    settings.bitmask_a = Bitmask(in.read_u32_le());
}

void BMP::Decoder::decode_info_header_v4()
{
    decode_info_header_v3();
    settings.color_space_type = (LogicalColorSpace)in.read_u32_le();
    settings.endpoints.r.x = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.r.y = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.r.z = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.g.x = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.g.y = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.g.z = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.b.x = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.b.y = uint32_to_float2_30(in.read_u32_le());
    settings.endpoints.b.z = uint32_to_float2_30(in.read_u32_le());
    // TODO: Fix wrong type conversion here from unsigned to float.
    settings.gamma_r = in.read_u32_le();
    settings.gamma_g = in.read_u32_le();
    settings.gamma_b = in.read_u32_le();
}

void BMP::Decoder::decode_info_header_v5()
{
    decode_info_header_v4();
    settings.gamut_mapping_intent = (GamutMappingIntent)in.read_u32_le();
    settings.profile_data = in.read_u32_le();
    settings.profile_size = in.read_u32_le();
    in.read_u32_le();
}

void BMP::Decoder::decode_color_table()
{
    settings.colors_table.reserve(settings.colors_count);
    for (uint32_t i = 0; i < settings.colors_count; ++i)
    {
        Pixel color = read_one_pixel_r8g8b8();
        settings.colors_table.push_back(color);
        in.read_u8();
    }
}

void BMP::Decoder::decode_pixel_array()
{
    switch (settings.bit_count)
    {
        case BitCount::BITCOUNT_0:
            decode_pixel_array_up_to_4_bpp();
            break;
        case BitCount::BITCOUNT_1:
            decode_pixel_array_up_to_4_bpp();
            break;
        case BitCount::BITCOUNT_2:
            decode_pixel_array_up_to_4_bpp();
            break;
        case BitCount::BITCOUNT_3:
            decode_pixel_array_8_bpp();
            break;
        case BitCount::BITCOUNT_4:
            decode_pixel_array_16_bpp();
            break;
        case BitCount::BITCOUNT_5:
            decode_pixel_array_24_bpp();
            break;
        case BitCount::BITCOUNT_6:
            decode_pixel_array_32_bpp();
            break;
    }
}

void BMP::Decoder::decode_pixel_array_up_to_4_bpp()
{
    const uint16_t BPP = (uint16_t)settings.bit_count;
    const int8_t BASE_OFFSET = 8 - BPP;
    const uint8_t BITMASK = pow(2, BPP) - 1;

    for (uint32_t y = 0; y < image.height; ++y)
    {
        uint8_t value = in.read_u8();
        uint32_t decoded_bytes = 1;
        int8_t offset = BASE_OFFSET;

        for (uint32_t x = 0; x < image.width; ++x)
        {
            decode_one_pixel_color_table((value >> offset) & BITMASK);
            if (offset == 0)
            {
                value = in.read_u8();
                ++decoded_bytes;
                offset = BASE_OFFSET;
            }
            else
            {
                offset -= BPP;
            }
        }

        for (; decoded_bytes % 4 != 0; ++decoded_bytes)
            in.read_u8();
    }
}

void BMP::Decoder::decode_pixel_array_8_bpp()
{
    for (uint32_t y = 0; y < image.height; ++y)
    {
        for (uint32_t x = 0; x < image.width; ++x)
            decode_one_pixel_color_table(in.read_u8());

        for (uint32_t x = image.width; x % 4 != 0; ++x)
            in.read_u8();
    }
}

void BMP::Decoder::decode_pixel_array_16_bpp()
{
    if (settings.compression == Compression::RGB)
    {
        settings.bitmask_a = Bitmask(0b0000000000000000);
        settings.bitmask_r = Bitmask(0b0000000000111110);
        settings.bitmask_g = Bitmask(0b0000011111000000);
        settings.bitmask_b = Bitmask(0b1111100000000000);
    }

    for (uint32_t y = 0; y < image.height; ++y)
    {
        for (uint32_t x = 0; x < image.width; ++x)
            decode_one_pixel_bitmask(in.read_u16_le());

        if (image.width % 2 != 0)
            in.read_u16();
    }
}

void BMP::Decoder::decode_pixel_array_24_bpp()
{
    if (settings.compression == Compression::BITFIELDS)
        throw DecodeException("The bitfields compression method is not allowed for 24bpp images.");

    for (uint32_t y = 0; y < image.height; ++y)
    {
        for (uint32_t x = 0; x < image.width; ++x)
            image.pixels.push_back(read_one_pixel_r8g8b8());

        for (uint32_t decoded_bytes = image.width * 3; decoded_bytes % 4 != 0; ++decoded_bytes)
            in.read_u8();
    }
}

void BMP::Decoder::decode_pixel_array_32_bpp()
{
    if (settings.compression == Compression::RGB)
    {
        settings.bitmask_a = Bitmask(0x00000000);
        settings.bitmask_r = Bitmask(0x0000ff00);
        settings.bitmask_g = Bitmask(0x00ff0000);
        settings.bitmask_b = Bitmask(0xff000000);
    }

    for (uint32_t i = 0; i < image.width * image.height; ++i)
        decode_one_pixel_bitmask(in.read_u32_le());
}

void BMP::Decoder::decode_one_pixel_bitmask(uint32_t value)
{
    Pixel pixel(
        ((value & settings.bitmask_r.value) >> settings.bitmask_r.offset) / settings.bitmask_r.divider * 255,
        ((value & settings.bitmask_g.value) >> settings.bitmask_g.offset) / settings.bitmask_g.divider * 255,
        ((value & settings.bitmask_b.value) >> settings.bitmask_b.offset) / settings.bitmask_b.divider * 255);

    if (settings.bitmask_a.value != 0)
        pixel.a = ((value & settings.bitmask_a.value) >> settings.bitmask_a.offset) / settings.bitmask_a.divider * 255;

    image.pixels.push_back(pixel);
}

void BMP::Decoder::decode_one_pixel_color_table(uint8_t value)
{
    Pixel pixel = settings.colors_table[value];
    image.pixels.push_back(pixel);
}

Pixel BMP::Decoder::read_one_pixel_r8g8b8()
{
    Pixel pixel;
    pixel.b = in.read_u8();
    pixel.g = in.read_u8();
    pixel.r = in.read_u8();
    pixel.a = 255;
    return pixel;
}
