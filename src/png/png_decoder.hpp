#pragma once

#include <cstddef>
#include <vector>

#include "decoder.hpp"
#include "png_common.hpp"
#include "png_settings.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace PNG {

class Decoder final : public BaseDecoder
{
private:
    Settings settings;

    BufferStream compressed_image_data {};
    std::vector<uint8_t> uncompressed_image_data {};

public:
    Decoder(InputStream &in, RawImage &image) :
        BaseDecoder(in, image) {}

    static bool can_decode(InputStream &in);

    void decode() override;

    Settings get_settings() const { return settings; }

private:
    void decode_chunk();

    void decode_chunk_header(uint32_t chunk_size);
    void decode_chunk_palette(uint32_t chunk_size);
    void decode_chunk_image_data(uint32_t chunk_size);
    void decode_chunk_image_trailer(uint32_t chunk_size);

    void decode_chunk_primary_chroma_and_white_points(uint32_t chunk_size);
    void decode_chunk_image_gamma(uint32_t chunk_size);
    void decode_chunk_icc_profile(uint32_t chunk_size);
    void decode_chunk_significant_bit(uint32_t chunk_size);
    void decode_chunk_standard_rgb(uint32_t chunk_size);
    void decode_chunk_background_color(uint32_t chunk_size);
    void decode_chunk_image_histogram(uint32_t chunk_size);
    void decode_chunk_transparency(uint32_t chunk_size);
    void decode_chunk_physical_pixel_dimensions(uint32_t chunk_size);
    void decode_chunk_suggested_palette(uint32_t chunk_size);
    void decode_chunk_image_last_modification_time(uint32_t chunk_size);
    void decode_chunk_textual_data(uint32_t chunk_size);
    void decode_chunk_compressed_textual_data(uint32_t chunk_size);
    void decode_chunk_international_textual_data(uint32_t chunk_size);

    void read_scanline();
    void read_scanline_byte();

    std::string decode_string() const;
    std::string decode_string_of_size(size_t size) const;

    void assert_chunk_size(uint32_t expected, uint32_t current, const std::string &chunk_name);

    void uncompress_image_data();
    void unfilter_image_data();
    void unserialize_scanlines();
};

}
