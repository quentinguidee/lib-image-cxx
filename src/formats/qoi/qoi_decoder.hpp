#pragma once

#include "qoi_chunks.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Decoder final : public BaseDecoder
{
    void decode(InputStream &in, RawImage &image) const override;
};

}
