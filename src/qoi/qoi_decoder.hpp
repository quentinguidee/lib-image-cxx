#pragma once

#include "qoi_common.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Decoder final : public BaseDecoder
{
public:
    void decode(InputStream &in, RawImage &image) const override;

    void decode_header(InputStream &in, RawImage &image) const;
    uint8_t decode_index(InputStream &in) const;
    void decode_diff(InputStream &in, Pixel &current_pixel) const;
    void decode_luma(InputStream &in, Pixel &current_pixel) const;
    void decode_rgb(InputStream &in, Pixel &pixel) const;
    void decode_rgba(InputStream &in, Pixel &pixel) const;
    uint8_t decode_run(InputStream &in) const;
};

}
