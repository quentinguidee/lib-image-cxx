#include "bmp_encoder.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

#include "bits.hpp"
#include "bmp_common.hpp"
#include "exceptions.hpp"
#include "log.hpp"

void BMP::Encoder::encode()
{
    generate_colors_table_indexes();

    if (settings.start_decoding_position == StartDecodingPosition::BottomLeft)
        image.flip_vertically();

    encode_header();

    uint32_t header_size = (uint32_t)settings.header_version;
    out.write_u32_le(header_size);
    switch (settings.header_version)
    {
        case HeaderVersion::CORE_HEADER:
            encode_core_header();
            break;
        case HeaderVersion::INFO_HEADER_V1:
            encode_info_header_v1();
            break;
        case HeaderVersion::INFO_HEADER_V2:
            encode_info_header_v2();
            break;
        case HeaderVersion::INFO_HEADER_V3:
            encode_info_header_v3();
            break;
        case HeaderVersion::INFO_HEADER_V4:
            encode_info_header_v4();
            break;
        case HeaderVersion::INFO_HEADER_V5:
            encode_info_header_v5();
            break;
        default:
            throw UnsupportedVersionException("Cannot encode headers of size " + std::to_string(header_size) + ".");
    }

    encode_color_table();
    encode_pixel_array();

    if (settings.start_decoding_position == StartDecodingPosition::BottomLeft)
        image.flip_vertically();
}

void BMP::Encoder::encode_header()
{
    out.write_u16((uint16_t)settings.signature);
    out.write_u32_le(get_file_size());
    out.write_u16_le(0x0000);
    out.write_u16_le(0x0000);
    out.write_u32_le(get_pixel_array_offset());
}

void BMP::Encoder::encode_core_header()
{
    out.write_u16_le(image.width);
    out.write_u16_le(image.height);
    out.write_u16_le(1);
    out.write_u16_le((uint16_t)settings.bit_count);
}

void BMP::Encoder::encode_info_header_v1()
{
    out.write_i32_le(image.width);
    if (settings.start_decoding_position == StartDecodingPosition::TopLeft)
        out.write_i32_le(-image.height);
    else
        out.write_i32_le(image.height);
    out.write_u16_le(1);
    out.write_u16_le((uint16_t)settings.bit_count);
    out.write_u32_le((uint32_t)settings.compression);
    out.write_u32_le(get_pixel_array_size());
    out.write_i32_le(settings.x_pixels_per_meter);
    out.write_i32_le(settings.y_pixels_per_meter);
    out.write_u32_le(settings.colors_count);
    out.write_u32_le(settings.important_colors_count);
}

void BMP::Encoder::encode_info_header_v2()
{
    encode_info_header_v1();
    out.write_u32_le(settings.bitmask_r.value);
    out.write_u32_le(settings.bitmask_g.value);
    out.write_u32_le(settings.bitmask_b.value);
}

void BMP::Encoder::encode_info_header_v3()
{
    encode_info_header_v2();
    out.write_u32_le(settings.bitmask_a.value);
}

void BMP::Encoder::encode_info_header_v4()
{
    encode_info_header_v3();
    out.write_u32_le((uint32_t)settings.color_space_type);
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.r.x));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.r.y));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.r.z));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.g.x));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.g.y));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.g.z));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.b.x));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.b.y));
    out.write_u32_le(float2_30_to_uint32(settings.endpoints.b.z));
    out.write_u32_le(settings.gamma_r);
    out.write_u32_le(settings.gamma_g);
    out.write_u32_le(settings.gamma_b);
}

void BMP::Encoder::encode_info_header_v5()
{
    encode_info_header_v4();
    out.write_u32_le((uint32_t)settings.gamut_mapping_intent);
    out.write_u32_le(settings.profile_data);
    out.write_u32_le(settings.profile_size);
    out.write_u32_le(0);
}

void BMP::Encoder::encode_color_table()
{
    for (Pixel& pixel : settings.colors_table)
    {
        write_one_pixel_r8g8b8(pixel);
        out.write_u8(0);
    }
}

void BMP::Encoder::encode_pixel_array()
{
    switch (settings.bit_count)
    {
        case BitCount::BITCOUNT_0:
            encode_pixel_array_up_to_4_bpp();
            break;
        case BitCount::BITCOUNT_1:
            encode_pixel_array_up_to_4_bpp();
            break;
        case BitCount::BITCOUNT_2:
            encode_pixel_array_up_to_4_bpp();
            break;
        case BitCount::BITCOUNT_3:
            encode_pixel_array_8_bpp();
            break;
        case BitCount::BITCOUNT_4:
            encode_pixel_array_16_bpp();
            break;
        case BitCount::BITCOUNT_5:
            encode_pixel_array_24_bpp();
            break;
        case BitCount::BITCOUNT_6:
            encode_pixel_array_32_bpp();
            break;
    }
}

void BMP::Encoder::encode_pixel_array_up_to_4_bpp()
{
    const uint16_t BPP = (uint16_t)settings.bit_count;
    const int8_t BASE_OFFSET = 8 - BPP;
    const uint8_t BITMASK = generate_bitmask<uint8_t>(BPP);

    for (uint32_t y = 0; y < image.height; ++y)
    {
        uint8_t value = 0;
        uint32_t encoded_bytes = 0;
        int8_t offset = BASE_OFFSET;

        for (uint32_t x = 0; x < image.width; ++x)
        {
            const Pixel& pixel = image.get_pixel(x, y);
            const uint8_t index = colors_table_indexes[(pixel.r) + (pixel.g * 255) + (pixel.b * 255 * 255)];
            value |= (index & BITMASK) << offset;
            if (offset == 0)
            {
                out.write_u8(value);
                value = 0;
                ++encoded_bytes;
                offset = BASE_OFFSET;
            }
            else
            {
                offset -= BPP;
            }
        }

        if (offset != BASE_OFFSET)
        {
            out.write_u8(value);
            ++encoded_bytes;
        }

        for (; encoded_bytes % 4 != 0; ++encoded_bytes)
            out.write_u8(0x00);
    }
}

void BMP::Encoder::encode_pixel_array_8_bpp()
{
    for (uint32_t y = 0; y < image.height; ++y)
    {
        for (uint32_t x = 0; x < image.width; ++x)
        {
            const Pixel& pixel = image.get_pixel(x, y);
            const uint8_t index = colors_table_indexes[(pixel.r) + (pixel.g * 255) + (pixel.b * 255 * 255)];
            out.write_u8(index);
        }

        for (uint32_t j = image.width; j % 4 != 0; ++j)
            out.write_u8(0);
    }
}

void BMP::Encoder::encode_pixel_array_16_bpp()
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
        {
            uint16_t value = pixel_to_value<uint16_t>(
                image.get_pixel(x, y),
                settings.bitmask_r,
                settings.bitmask_g,
                settings.bitmask_b,
                settings.bitmask_a);

            out.write_u16_le(value);
        }

        if (image.width % 2 != 0)
            out.write_u16(0);
    }
}

void BMP::Encoder::encode_pixel_array_24_bpp()
{
    if (settings.compression == Compression::BITFIELDS)
        throw std::runtime_error("The bitfields compression method is not allowed for 24bpp images.");

    for (uint32_t y = 0; y < image.height; ++y)
    {
        for (uint32_t x = 0; x < image.width; ++x)
            write_one_pixel_r8g8b8(image.get_pixel(x, y));

        for (uint32_t decoded_bytes = image.width * 3; decoded_bytes % 4 != 0; ++decoded_bytes)
            out.write_u8(0);
    }
}

void BMP::Encoder::encode_pixel_array_32_bpp()
{
    if (settings.compression == Compression::RGB)
    {
        settings.bitmask_a = Bitmask(0x00000000);
        settings.bitmask_r = Bitmask(0x0000ff00);
        settings.bitmask_g = Bitmask(0x00ff0000);
        settings.bitmask_b = Bitmask(0xff000000);
    }

    for (uint32_t i = 0; i < image.width * image.height; ++i)
    {
        uint32_t value = pixel_to_value<uint32_t>(
            image.pixels[i],
            settings.bitmask_r,
            settings.bitmask_g,
            settings.bitmask_b,
            settings.bitmask_a);

        out.write_u32_le(value);
    }
}

void BMP::Encoder::write_one_pixel_r8g8b8(const Pixel& pixel)
{
    out.write_u8(pixel.b);
    out.write_u8(pixel.g);
    out.write_u8(pixel.r);
}

uint32_t BMP::Encoder::get_pixel_array_offset() const
{
    uint32_t header_size = FILE_HEADER_SIZE + (uint8_t)settings.header_version;
    uint32_t color_table_size = settings.colors_count;
    uint32_t starting_address = header_size + color_table_size;
    if (starting_address < settings.starting_address)
        return settings.starting_address;

    return starting_address;
}

uint32_t BMP::Encoder::get_pixel_array_size() const
{
    uint32_t line_size = image.width * (uint16_t)settings.bit_count;
    if (line_size % 32 != 0)
        line_size += 32 - (line_size % 32);
    return (line_size / 8) * image.height;
}

uint32_t BMP::Encoder::get_file_size() const
{
    return get_pixel_array_offset() + get_pixel_array_size();
}

void BMP::Encoder::generate_colors_table_indexes()
{
    colors_table_indexes.clear();
    colors_table_indexes.reserve(settings.colors_table.size());
    for (uint32_t i = 0; i < settings.colors_table.size(); ++i)
    {
        const Pixel& pixel = settings.colors_table[i];
        const uint32_t hash = (pixel.r) + (pixel.g * 255) + (pixel.b * 255 * 255);
        colors_table_indexes.insert({hash, i});
    }
}
