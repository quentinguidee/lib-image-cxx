#pragma once

#include "decoder.hpp"
#include "qoi_common.hpp"
#include "qoi_settings.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Decoder final : public BaseDecoder
{
private:
    Settings settings {};

public:
    Decoder(InputStream &in, RawImage &image) :
        BaseDecoder { in, image } {}

    static bool can_decode(InputStream &in);

    void decode() override;

    Settings get_settings() const { return settings; }

private:
    void decode_header();
    uint8_t decode_index();
    void decode_diff(Pixel &current_pixel);
    void decode_luma(Pixel &current_pixel);
    void decode_rgb(Pixel &pixel);
    void decode_rgba(Pixel &pixel);
    uint8_t decode_run();
};

}
