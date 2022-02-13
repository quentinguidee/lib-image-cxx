#pragma once

#include "qoi_common.hpp"
#include "qoi_settings.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Encoder final : public BaseEncoder
{
private:
    Settings settings;

public:
    Encoder(OutputStream &out, RawImage &image) :
        BaseEncoder(out, image) {}

    void encode() override;

    void set_settings(const Settings &settings) { this->settings = settings; }

private:
    void encode_header() const;
    void encode_index(uint8_t index) const;
    void encode_diff(int8_t diff_r, int8_t diff_g, int8_t diff_b) const;
    void encode_luma(int8_t diff_g, int8_t diff_r_g, int8_t diff_b_g) const;
    void encode_rgb(const Pixel &pixel) const;
    void encode_rgba(const Pixel &pixel) const;
    void encode_run(uint8_t run) const;
    void encode_footer() const;
};

}
