#pragma once

#include "png_common.hpp"
#include "png_settings.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace PNG {

class Encoder final : public BaseEncoder
{
private:
    Settings settings;

public:
    Encoder(OutputStream &out, RawImage &image) :
        BaseEncoder(out, image) {}

    void encode() override;

    void set_settings(const Settings &settings) { this->settings = settings; }
};

}
