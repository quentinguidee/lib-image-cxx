#pragma once

#include <memory>

#include "bmp_decoder.hpp"
#include "bmp_encoder.hpp"
#include "decoder.hpp"
#include "encoder.hpp"
#include "format.hpp"

static const Format BMP_FORMAT{
    "bmp",
    "Bitmap",
    {"bmp"},
};
