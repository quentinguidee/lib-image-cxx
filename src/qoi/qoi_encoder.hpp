#pragma once

#include "qoi_common.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Encoder final : public BaseEncoder
{
public:
    void encode(OutputStream &out, RawImage &image) const override;

    void encode_header(OutputStream &out, const RawImage &image) const;
    void encode_index(OutputStream &out, uint8_t index) const;
    void encode_diff(OutputStream &out, int8_t diff_r, int8_t diff_g, int8_t diff_b) const;
    void encode_luma(OutputStream &out, int8_t diff_g, int8_t diff_r_g, int8_t diff_b_g) const;
    void encode_rgb(OutputStream &out, const Pixel &pixel) const;
    void encode_rgba(OutputStream &out, const Pixel &pixel) const;
    void encode_run(OutputStream &out, uint8_t run) const;
    void encode_footer(OutputStream &out) const;
};

}
