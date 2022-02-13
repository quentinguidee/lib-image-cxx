#pragma once

#include <string>
#include <utility>

#include "pixel.hpp"
#include "png_common.hpp"

namespace PNG {

struct Settings
{
    // Header
    BitDepth bit_depth;
    ColorType color_type;
    CompressionMethod compression_method;
    FilterMethod filter_method;
    InterlaceMethod interlace_method;

    // Palette
    std::vector<Pixel> palette;

    // Primary chroma and white points
    double white_point_x, white_point_y;
    double red_x, red_y;
    double green_x, green_y;
    double blue_x, blue_y;

    // Gamma
    double gamma;

    // ICC Profile
    std::string icc_profile_name;
    CompressionMethod icc_compression_method;

    // Significant bits
    uint8_t significant_greyscale_bits;
    uint8_t significant_red_bits;
    uint8_t significant_green_bits;
    uint8_t significant_blue_bits;
    uint8_t significant_alpha_bits;

    // Standard RGB
    RenderingIntent rendering_intent;

    // Text data
    std::vector<TextualData> textual_data {};
    std::vector<CompressedTextualData> compressed_textual_data {};
    std::vector<InternationalTextualData> international_textual_data {};

    // Background
    uint16_t background_greyscale;
    uint16_t background_red;
    uint16_t background_green;
    uint16_t background_blue;
    uint8_t background_palette_index;

    // Physical pixel dimensions
    uint32_t pixels_per_unit_x, pixels_per_unit_y;
    UnitSpecifier unit_specifier;

    // Last modification date
    Date last_modification_date;
};

}
