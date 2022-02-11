#pragma once

#include <unordered_map>

#include "bmp_common.hpp"
#include "bmp_settings.hpp"
#include "encoder.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace BMP {

class Encoder final : public BaseEncoder
{
private:
    Settings settings;

    // r + 255 g + 255^2 b => colors_table index
    std::unordered_map<uint32_t, uint32_t> colors_table_indexes;

public:
    Encoder(OutputStream &out, RawImage &image) :
        BaseEncoder(out, image) {}

    void encode() override;

    void set_settings(const Settings &settings) { this->settings = settings; }

private:
    void encode_header();
    void encode_core_header();
    void encode_info_header_v1();
    void encode_info_header_v2();
    void encode_info_header_v3();
    void encode_info_header_v4();
    void encode_info_header_v5();

    void encode_color_table();

    void encode_pixel_array();
    void encode_pixel_array_up_to_4_bpp();
    void encode_pixel_array_8_bpp();
    void encode_pixel_array_16_bpp();
    void encode_pixel_array_24_bpp();
    void encode_pixel_array_32_bpp();
    void encode_one_pixel_bitmask(const Pixel &pixel);
    void write_one_pixel_r8g8b8(const Pixel &pixel);

    uint32_t get_pixel_array_offset() const;
    uint32_t get_pixel_array_size() const;
    uint32_t get_file_size() const;

    void generate_colors_table_indexes();
};

}
