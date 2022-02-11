#pragma once

#include <memory>

#include "decoder.hpp"
#include "encoder.hpp"
#include "format.hpp"
#include "qoi_decoder.hpp"
#include "qoi_encoder.hpp"

static const Format QOI_FORMAT{
    "qoi",
    "Quite OK image format",
    {"qoi"},
};
