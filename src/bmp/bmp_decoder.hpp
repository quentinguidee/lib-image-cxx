#pragma once

#include "bmp_common.hpp"
#include "bmp_settings.hpp"
#include "decoder.hpp"
#include "raw_image.hpp"

namespace BMP {

class Decoder final : public BaseDecoder
{
private:
    Settings settings;

    uint32_t file_size;
    uint32_t image_size;

public:
    Decoder(InputStream &in, RawImage &image) :
        BaseDecoder(in, image) {}

    static bool can_decode(InputStream &in);

    void decode() override;

    Settings get_settings() const { return settings; }

private:
    void decode_header();
    void decode_core_header();
    void decode_info_header_v1();
    void decode_info_header_v2();
    void decode_info_header_v3();
    void decode_info_header_v4();
    void decode_info_header_v5();

    void decode_color_table();

    void decode_pixel_array();
    void decode_pixel_array_up_to_4_bpp();
    void decode_pixel_array_8_bpp();
    void decode_pixel_array_16_bpp();
    void decode_pixel_array_24_bpp();
    void decode_pixel_array_32_bpp();
    void decode_one_pixel_bitmask(uint32_t value);
    void decode_one_pixel_color_table(uint8_t value);
    Pixel read_one_pixel_r8g8b8();
};

}
