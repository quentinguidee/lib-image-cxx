#pragma once

#include "decoder.hpp"
#include "qoi_common.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Decoder final : public BaseDecoder
{
public:
    Decoder(InputStream &in, RawImage &image) :
        BaseDecoder(in, image) {}

    static bool can_decode(InputStream &in);

    void decode() override;

    void decode_header() const;
    uint8_t decode_index() const;
    void decode_diff(Pixel &current_pixel) const;
    void decode_luma(Pixel &current_pixel) const;
    void decode_rgb(Pixel &pixel) const;
    void decode_rgba(Pixel &pixel) const;
    uint8_t decode_run() const;
};

}
