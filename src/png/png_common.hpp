#pragma once

#include <cstdint>
#include <string>

#include "pixel.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace PNG {

static constexpr uint64_t SIGNATURE = 0x89504E470D0A1A0A;

// clang-format off
static constexpr uint32_t CHUNK_NAME_HEADER                            = 0x49484452;  // IHDR
static constexpr uint32_t CHUNK_NAME_PALETTE                           = 0x504C5445;  // PLTE
static constexpr uint32_t CHUNK_NAME_IMAGE_DATA                        = 0x49444154;  // IDAT
static constexpr uint32_t CHUNK_NAME_IMAGE_TRAILER                     = 0x49454E44;  // IEND

static constexpr uint32_t CHUNK_NAME_PRIMARY_CHROMA_AND_WHITE_POINTS   = 0x6348524D;  // cHRM
static constexpr uint32_t CHUNK_NAME_IMAGE_GAMMA                       = 0x67414D41;  // gAMA
static constexpr uint32_t CHUNK_NAME_ICC_PROFILE                       = 0x69434350;  // iCCP
static constexpr uint32_t CHUNK_NAME_SIGNIFICANT_BIT                   = 0x73424954;  // sBIT
static constexpr uint32_t CHUNK_NAME_STANDARD_RGB                      = 0x73524742;  // sRGB
static constexpr uint32_t CHUNK_NAME_BACKGROUND_COLOR                  = 0x624B4744;  // bKGD
static constexpr uint32_t CHUNK_NAME_IMAGE_HISTOGRAM                   = 0x68495354;  // hIST
static constexpr uint32_t CHUNK_NAME_TRANSPARENCY                      = 0x74524E53;  // tRNS
static constexpr uint32_t CHUNK_NAME_PHYSICAL_PIXEL_DIMENSIONS         = 0x70485973;  // pHYs
static constexpr uint32_t CHUNK_NAME_SUGGESTED_PALETTE                 = 0x73504C54;  // sPLT
static constexpr uint32_t CHUNK_NAME_IMAGE_LAST_MODIFICATION_TIME      = 0x74494D45;  // tIME
static constexpr uint32_t CHUNK_NAME_INTERNATIONAL_TEXTUAL_DATA        = 0x69545874;  // iTXt
static constexpr uint32_t CHUNK_NAME_TEXTUAL_DATA                      = 0x74455874;  // tEXt
static constexpr uint32_t CHUNK_NAME_COMPRESSED_TEXTUAL_DATA           = 0x7A545874;  // zTXt
// clang-format on

enum class BitDepth : uint8_t
{
    BD_1 = 1,
    BD_2 = 2,
    BD_4 = 4,
    BD_8 = 8,
    BD_16 = 16,
};

enum class ColorType : uint8_t
{
    GREYSCALE = 0,
    TRUE_COLOR = 2,
    INDEXED_COLOR = 3,
    GREYSCALE_WITH_ALPHA = 4,
    TRUE_COLOR_WITH_ALPHA = 6,
};

enum class CompressionMethod : uint8_t
{
    DEFLATE_INFLATE = 0,
};

enum class FilterMethod : uint8_t
{
    ADAPTIVE = 0,
};

enum class FilterType : uint8_t
{
    NONE = 0,
    SUB = 1,
    UP = 2,
    AVERAGE = 3,
    PAETH = 4,
};

enum class InterlaceMethod : uint8_t
{
    STANDARD = 0,
    ADAM_7 = 1,
};

enum class RenderingIntent : uint8_t
{
    PERCEPTUAL = 0,
    RELATIVE_COLORIMETRIC = 1,
    SATURATION = 2,
    ABSOLUTE_COLORIMETRIC = 3,
};

enum class UnitSpecifier : uint8_t
{
    UNKNOWN = 0,
    METER = 1,
};

struct Date
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

class TextualData
{
protected:
    std::string keyword;
    std::string text;

public:
    TextualData(const std::string& keyword, const std::string& text) :
        keyword { keyword }, text { text } {}

    virtual ~TextualData() = default;

    const std::string& get_keyword() const { return keyword; }
    virtual const std::string& get_text() const { return text; }
};

class CompressedTextualData : public TextualData
{
protected:
    uint8_t compression_method;

public:
    CompressedTextualData(const std::string& keyword, const std::string& text, uint8_t compression_method) :
        TextualData { keyword, text }, compression_method { compression_method } {}

    virtual ~CompressedTextualData() = default;

    // TODO: Uncompress text
    const std::string& get_text() const override { return text; }
};

struct InternationalTextualData : public CompressedTextualData
{
protected:
    std::string language_tag;
    uint8_t compression_flag;

public:
    InternationalTextualData(const std::string& keyword, const std::string& text, uint8_t compression_flag, uint8_t compression_method, const std::string& language_tag) :
        CompressedTextualData { keyword, text, compression_method },
        language_tag { language_tag },
        compression_flag { compression_flag }
    {
    }

    virtual ~InternationalTextualData() = default;

    const std::string& get_language_tag() const { return language_tag; }
    uint8_t get_compression_flag() const { return compression_flag; }
};

}
