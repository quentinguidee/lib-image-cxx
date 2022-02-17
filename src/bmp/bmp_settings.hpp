#pragma once

#include <cstdint>
#include <map>

#include "bmp_common.hpp"
#include "pixel.hpp"

namespace BMP {

struct Settings
{
    // Header
    HeaderVersion header_version { HeaderVersion::INFO_HEADER_V5 };
    Signature signature { Signature::BM };
    uint32_t starting_address;

    // Header core + info v1
    StartDecodingPosition start_decoding_position { StartDecodingPosition::TopLeft };
    BitCount bit_count { BitCount::BITCOUNT_6 };
    Compression compression { Compression::RGB };
    int32_t x_pixels_per_meter, y_pixels_per_meter;
    uint32_t colors_count { 0 };
    uint32_t important_colors_count { 0 };

    // Header info v2
    Bitmask bitmask_r, bitmask_g, bitmask_b;

    // Header info v3
    Bitmask bitmask_a;

    // Header info v4
    LogicalColorSpace color_space_type { LogicalColorSpace::SRGB };
    CIEXYZTriple endpoints;
    float gamma_r, gamma_g, gamma_b;

    // Header info v5
    GamutMappingIntent gamut_mapping_intent;
    uint32_t profile_data;
    uint32_t profile_size;

    // Color table
    std::vector<Pixel> colors_table {};
};

}
