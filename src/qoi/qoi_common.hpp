#pragma once

#include <cstdint>

#include "pixel.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

constexpr uint32_t HEADER_MAGIC = 0x716f6966;
constexpr uint8_t INDEX_TAG = 0x00;
constexpr uint8_t DIFF_TAG = 0x40;
constexpr uint8_t RGB_TAG = 0xfe;
constexpr uint8_t RGBA_TAG = 0xff;
constexpr uint8_t LUMA_TAG = 0x80;
constexpr uint8_t RUN_TAG = 0xc0;

uint8_t hash_pixel(const Pixel& pixel);

enum class Colorspace
{
    SRGB,
    LINEAR,
};

enum class Channels
{
    RGB,
    RGBA,
};

}
