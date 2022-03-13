#include "png_decoder.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "bits.hpp"
#include "deflate_compressor.hpp"
#include "exceptions.hpp"
#include "log.hpp"
#include "pixel.hpp"
#include "png_common.hpp"

namespace PNG {

bool Decoder::can_decode(InputStream &in)
{
    return in.peek<uint64_t>(8) == SIGNATURE;
}

void Decoder::decode()
{
    // Signature
    in.read<uint64_t>(8);

    while (in.remaining_size() > 0)
        decode_chunk();

    uncompress_image_data();
    unfilter_image_data();
    unserialize_scanlines();
}

void Decoder::decode_chunk()
{
    const uint32_t chunk_size = in.read_u32();
    const uint32_t chunk_name = in.read_u32();
    switch (chunk_name)
    {
        case CHUNK_NAME_HEADER:
            decode_chunk_header(chunk_size);
            break;
        case CHUNK_NAME_PALETTE:
            decode_chunk_palette(chunk_size);
            break;
        case CHUNK_NAME_IMAGE_DATA:
            decode_chunk_image_data(chunk_size);
            break;
        case CHUNK_NAME_IMAGE_TRAILER:
            decode_chunk_image_trailer(chunk_size);
            break;
        case CHUNK_NAME_PRIMARY_CHROMA_AND_WHITE_POINTS:
            decode_chunk_primary_chroma_and_white_points(chunk_size);
            break;
        case CHUNK_NAME_IMAGE_GAMMA:
            decode_chunk_image_gamma(chunk_size);
            break;
        case CHUNK_NAME_ICC_PROFILE:
            decode_chunk_icc_profile(chunk_size);
            break;
        case CHUNK_NAME_SIGNIFICANT_BIT:
            decode_chunk_significant_bit(chunk_size);
            break;
        case CHUNK_NAME_STANDARD_RGB:
            decode_chunk_standard_rgb(chunk_size);
            break;
        case CHUNK_NAME_BACKGROUND_COLOR:
            decode_chunk_background_color(chunk_size);
            break;
        case CHUNK_NAME_IMAGE_HISTOGRAM:
            decode_chunk_image_histogram(chunk_size);
            break;
        case CHUNK_NAME_TRANSPARENCY:
            decode_chunk_transparency(chunk_size);
            break;
        case CHUNK_NAME_PHYSICAL_PIXEL_DIMENSIONS:
            decode_chunk_physical_pixel_dimensions(chunk_size);
            break;
        case CHUNK_NAME_SUGGESTED_PALETTE:
            decode_chunk_suggested_palette(chunk_size);
            break;
        case CHUNK_NAME_IMAGE_LAST_MODIFICATION_TIME:
            decode_chunk_image_last_modification_time(chunk_size);
            break;
        case CHUNK_NAME_INTERNATIONAL_TEXTUAL_DATA:
            decode_chunk_international_textual_data(chunk_size);
            break;
        case CHUNK_NAME_TEXTUAL_DATA:
            decode_chunk_textual_data(chunk_size);
            break;
        case CHUNK_NAME_COMPRESSED_TEXTUAL_DATA:
            decode_chunk_compressed_textual_data(chunk_size);
            break;
        default:
            // LOG_ERROR("This " << std::hex << chunk_name << " chunk is not handled by the PNG decoder.");
            in.skip(chunk_size);
    }

    // TODO: Check CRC (uint32_t)
    in.skip(4);  // Cyclic redundancy code (CRC)
}

void Decoder::decode_chunk_header(uint32_t chunk_size)
{
    if (chunk_size != 13)
        throw std::runtime_error { "A chunk size different than 13 bytes is not supported." };

    image.width = in.read_u32();
    image.height = in.read_u32();
    settings.bit_depth = (BitDepth)in.read_u8();
    settings.color_type = (ColorType)in.read_u8();
    settings.compression_method = (CompressionMethod)in.read_u8();
    settings.filter_method = (FilterMethod)in.read_u8();
    settings.interlace_method = (InterlaceMethod)in.read_u8();
}

void Decoder::decode_chunk_palette(uint32_t chunk_size)
{
    if (chunk_size % 3 != 0)
        throw std::runtime_error { "Palette chunk size must be divisible by 3. chunk_size = " + std::to_string(chunk_size) };

    for (uint8_t i = 0; i < chunk_size / 3; ++i)
    {
        settings.palette.clear();
        settings.palette.push_back(Pixel(
            in.read_u8(),
            in.read_u8(),
            in.read_u8()));
    }
}

void Decoder::decode_chunk_image_data(uint32_t chunk_size)
{
    for (long i = 0; i < chunk_size; ++i)
        compressed_image_data.write_u8(in.read_u8());
}

void Decoder::decode_chunk_image_trailer(uint32_t chunk_size)
{
    assert_chunk_size(0, chunk_size, "Image gamma");
}

void Decoder::decode_chunk_primary_chroma_and_white_points(uint32_t chunk_size)
{
    assert_chunk_size(32, chunk_size, "Primary chroma and white points");

    const double DIVISION_FACTOR = 10000.0f;
    settings.white_point_x = in.read_u32() / DIVISION_FACTOR;
    settings.white_point_y = in.read_u32() / DIVISION_FACTOR;
    settings.red_x = in.read_u32() / DIVISION_FACTOR;
    settings.red_y = in.read_u32() / DIVISION_FACTOR;
    settings.green_x = in.read_u32() / DIVISION_FACTOR;
    settings.green_y = in.read_u32() / DIVISION_FACTOR;
    settings.blue_x = in.read_u32() / DIVISION_FACTOR;
    settings.blue_y = in.read_u32() / DIVISION_FACTOR;
}

void Decoder::decode_chunk_image_gamma(uint32_t chunk_size)
{
    assert_chunk_size(4, chunk_size, "Image gamma");

    const double DIVISION_FACTOR = 10000.0f;
    settings.gamma = in.read_u32() / DIVISION_FACTOR;
}

void Decoder::decode_chunk_icc_profile(uint32_t chunk_size)
{
    char character;
    while ((character = in.read_u8()) != '\0')
        settings.icc_profile_name += character;

    settings.icc_compression_method = (CompressionMethod)in.read_u8();

    // TODO: Support icc_compressed_profile
    in.skip(chunk_size - settings.icc_profile_name.size() - 2);
}

void Decoder::decode_chunk_significant_bit(uint32_t chunk_size)
{
    if (settings.color_type == ColorType::GREYSCALE && chunk_size == 1)
    {
        settings.significant_greyscale_bits = in.read_u8();
        return;
    }

    if ((settings.color_type == ColorType::TRUE_COLOR || settings.color_type == ColorType::INDEXED_COLOR) && chunk_size == 3)
    {
        settings.significant_red_bits = in.read_u8();
        settings.significant_green_bits = in.read_u8();
        settings.significant_blue_bits = in.read_u8();
        return;
    }

    if (settings.color_type == ColorType::GREYSCALE_WITH_ALPHA && chunk_size == 2)
    {
        settings.significant_greyscale_bits = in.read_u8();
        settings.significant_alpha_bits = in.read_u8();
        return;
    }

    if (settings.color_type == ColorType::TRUE_COLOR_WITH_ALPHA && chunk_size == 4)
    {
        settings.significant_red_bits = in.read_u8();
        settings.significant_green_bits = in.read_u8();
        settings.significant_blue_bits = in.read_u8();
        settings.significant_alpha_bits = in.read_u8();
        return;
    }

    throw std::runtime_error { "Wrong significant bits chunk_size, or unsupported color_type." };
}

void Decoder::decode_chunk_standard_rgb(uint32_t chunk_size)
{
    assert_chunk_size(1, chunk_size, "Standard RGB");

    settings.rendering_intent = (RenderingIntent)in.read_u8();

    settings.gamma = 45455;

    settings.white_point_x = 31270;
    settings.white_point_y = 32900;
    settings.red_x = 64000;
    settings.red_y = 33000;
    settings.green_x = 30000;
    settings.green_y = 60000;
    settings.blue_x = 15000;
    settings.blue_y = 6000;
}

void Decoder::decode_chunk_background_color(uint32_t chunk_size)
{
    if ((settings.color_type == ColorType::GREYSCALE || settings.color_type == ColorType::GREYSCALE_WITH_ALPHA) && chunk_size == 2)
    {
        settings.background_greyscale = in.read_u16();
        return;
    }

    if ((settings.color_type == ColorType::TRUE_COLOR || settings.color_type == ColorType::TRUE_COLOR_WITH_ALPHA) && chunk_size == 6)
    {
        settings.background_red = in.read_u16();
        settings.background_green = in.read_u16();
        settings.background_blue = in.read_u16();
        return;
    }

    if (settings.color_type == ColorType::INDEXED_COLOR && chunk_size == 2)
    {
        settings.background_palette_index = in.read_u8();
        return;
    }

    throw std::runtime_error { "Wrong background color chunk_size, or unsupported color_type." };
}

void Decoder::decode_chunk_image_histogram(uint32_t chunk_size)
{
    // TODO: Support histogram
    in.skip(chunk_size);
}

void Decoder::decode_chunk_transparency(uint32_t chunk_size)
{
    assert_chunk_size(1, chunk_size, "Transparency");
}

void Decoder::decode_chunk_physical_pixel_dimensions(uint32_t chunk_size)
{
    assert_chunk_size(9, chunk_size, "Physical pixel dimensions");

    settings.pixels_per_unit_x = in.read_u32();
    settings.pixels_per_unit_y = in.read_u32();
    settings.unit_specifier = (UnitSpecifier)in.read_u8();
}

void Decoder::decode_chunk_suggested_palette(uint32_t chunk_size)
{
    // TODO: Support suggested palette
    in.skip(chunk_size);
}

void Decoder::decode_chunk_image_last_modification_time(uint32_t chunk_size)
{
    assert_chunk_size(7, chunk_size, "Last modification date");

    settings.last_modification_date = {
        in.read_u16(),
        in.read_u8(),
        in.read_u8(),
        in.read_u8(),
        in.read_u8(),
        in.read_u8(),
    };
}

void Decoder::decode_chunk_textual_data(uint32_t chunk_size)
{
    std::string keyword = decode_string();

    uint8_t remaining_bytes = chunk_size - keyword.size() - 1;
    std::string text = decode_string_of_size(remaining_bytes);

    settings.textual_data.push_back({
        keyword,
        text,
    });
}

void Decoder::decode_chunk_compressed_textual_data(uint32_t chunk_size)
{
    std::string keyword = decode_string();

    uint8_t compression_method = in.read_u8();

    uint8_t remaining_bytes = chunk_size - keyword.size() - 1;
    std::string text = decode_string_of_size(remaining_bytes);

    settings.compressed_textual_data.push_back({
        keyword,
        text,
        compression_method,
    });
}

void Decoder::decode_chunk_international_textual_data(uint32_t chunk_size)
{
    std::string keyword = decode_string();

    uint8_t compression_flag = in.read_u8();
    uint8_t compression_method = in.read_u8();

    std::string language_tag = decode_string();
    std::string translated_keyword = decode_string();

    uint8_t remaining_bytes = chunk_size - keyword.size() - language_tag.size() - translated_keyword.size() - 5;

    std::string text = decode_string_of_size(remaining_bytes);

    settings.international_textual_data.push_back({
        keyword,
        text,
        compression_flag,
        compression_method,
        language_tag,
    });
}

std::string Decoder::decode_string() const
{
    std::string value;
    char character;
    while ((character = in.read_u8()) != '\0')
        value += character;

    return value;
}

std::string Decoder::decode_string_of_size(size_t size) const
{
    std::string value;
    for (size_t i = 0; i < size; ++i)
        value += in.read_u8();

    return value;
}

void Decoder::assert_chunk_size(uint32_t expected, uint32_t current, const std::string &chunk_name)
{
    if (expected == current) return;
    throw std::runtime_error { chunk_name + " chunk size must be " + std::to_string(expected) + ". Current = " + std::to_string(current) };
}

void Decoder::uncompress_image_data()
{
    uncompressed_image_data = Compression::DeflateCompressor(compressed_image_data).uncompress();
}

void Decoder::unfilter_image_data()
{
    if (settings.filter_method != FilterMethod::ADAPTIVE)
        throw std::runtime_error { "The adaptive filter method is the only one supported." };

    /*
    for (auto it = image_data.begin(); it != image_data.end(); ++it)
    {
        uint8_t filter_type = 0;
        switch (filter_type)
        {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                throw std::runtime_error { "The filter_type " + std::to_string(filter_type) + "doesn't exist." };
        }
    }
    */
}

void Decoder::unserialize_scanlines()
{
}

}
