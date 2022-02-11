#pragma once

#include <cstdint>

#include "stream.hpp"

namespace BMP {

constexpr uint8_t FILE_HEADER_SIZE = 14;
constexpr uint8_t CORE_HEADER_SIZE = 12;
constexpr uint8_t INFO_HEADER_V1_SIZE = 40;
constexpr uint8_t INFO_HEADER_V2_SIZE = 52;
constexpr uint8_t INFO_HEADER_V3_SIZE = 56;
constexpr uint8_t INFO_HEADER_V4_SIZE = 108;
constexpr uint8_t INFO_HEADER_V5_SIZE = 124;

enum class HeaderVersion : uint8_t
{
    CORE_HEADER = CORE_HEADER_SIZE,
    INFO_HEADER_V1 = INFO_HEADER_V1_SIZE,
    INFO_HEADER_V2 = INFO_HEADER_V2_SIZE,
    INFO_HEADER_V3 = INFO_HEADER_V3_SIZE,
    INFO_HEADER_V4 = INFO_HEADER_V4_SIZE,
    INFO_HEADER_V5 = INFO_HEADER_V5_SIZE,
};

enum class BitCount : uint16_t
{
    BITCOUNT_0 = 0x0000,  // 1bpp
    BITCOUNT_1 = 0x0001,  // 2bpp
    BITCOUNT_2 = 0x0004,  // 4bpp
    BITCOUNT_3 = 0x0008,  // 8bpp
    BITCOUNT_4 = 0x0010,  // 16bpp
    BITCOUNT_5 = 0x0018,  // 24bpp
    BITCOUNT_6 = 0x0020,  // 32bpp
};

enum class Compression : uint32_t
{
    RGB = 0x0000,
    RLE8 = 0x0001,
    RLE4 = 0x0002,
    BITFIELDS = 0x0003,
    JPEG = 0x0004,
    PNG = 0x0005,
    CMYK = 0x000b,
    CMYK_RLE8 = 0x000c,
    CMYK_RLE4 = 0x000d,
};

enum class Signature : uint16_t
{
    BM = 0x424d,
};

enum class LogicalColorSpace : uint32_t
{
    CALIBRATED_RGB = 0x00000000,
    SRGB = 0x73524742,
    WINDOWS_COLORSPACE = 0x57696e20,
};

union float2_30_t {
    uint32_t value;
    struct
    {
        uint8_t integer : 2;
        uint32_t floating : 30;
    };
};

float2_30_t uint32_to_float2_30(uint32_t value);
uint32_t float2_30_to_uint32(float2_30_t value);

struct CIEXYZ
{
    float2_30_t x, y, z;
};

struct CIEXYZTriple
{
    CIEXYZ r, g, b;
};

enum class GamutMappingIntent : uint32_t
{
    ABS_COLORIMETRIC = 0x00000008,
    BUSINESS = 0x00000001,
    GRAPHICS = 0x00000002,
    IMAGES = 0x00000004,
};

enum class StartDecodingPosition
{
    TopLeft,
    BottomLeft,
};

struct Bitmask
{
    uint32_t value;
    uint8_t offset;
    double divider;

    Bitmask(uint32_t value = 0);
};

}
