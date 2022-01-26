#pragma once

#include "decoder.hpp"
#include "qoi_chunks.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Encoder final : public BaseEncoder
{
    void encode(OutputStream &out, RawImage &image) const override;
};

}
