#pragma once

#include <cstdint>
#include <vector>

#include "pixel.hpp"
#include "raw_image.hpp"
#include "stream.hpp"

namespace QOI {

class Chunk
{
    virtual void encode(OutputStream& out) = 0;
    virtual void decode(InputStream& in) = 0;
};

class Header final : public Chunk
{
public:
    static constexpr uint32_t MAGIC = 'qoif';
    static constexpr uint8_t SIZE = 14;

private:
    uint32_t width;
    uint32_t height;
    Channels channels;
    Colorspace colorspace;

public:
    Header() {}
    Header(InputStream& in);
    Header(uint32_t width, uint32_t height, Channels channels, Colorspace colorspace);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    uint32_t get_width() const { return width; }
    uint32_t get_height() const { return height; }
    Channels get_channels() const { return channels; }
    Colorspace get_colorspace() const { return colorspace; }

    bool operator==(const Header& rhs) const;
};

class Index final : public Chunk
{
public:
    static constexpr uint8_t TAG = 0x00;
    static constexpr uint8_t SIZE = 1;

private:
    uint8_t index;

public:
    Index(InputStream& in);
    Index(uint8_t index);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    uint8_t get_index() const { return index; }

    bool operator==(const Index& rhs) const;
};

class Diff final : public Chunk
{
public:
    static constexpr uint8_t TAG = 0x40;
    static constexpr uint8_t SIZE = 1;

private:
    uint8_t diff_red, diff_green, diff_blue;

public:
    Diff(InputStream& in);
    Diff(int8_t red_diff, int8_t green_diff, int8_t blue_diff);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    int8_t get_diff_red() const { return diff_red - 2; }
    int8_t get_diff_green() const { return diff_green - 2; }
    int8_t get_diff_blue() const { return diff_blue - 2; }

    bool operator==(const Diff& rhs) const;
};

struct Color
{
    uint8_t r, g, b, a;

    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

    uint8_t hash() const;
    Pixel to_pixel() const { return {r, g, b, a}; }

    bool operator==(const Color& rhs) const;
};

class RGB final : public Chunk
{
public:
    static constexpr uint8_t TAG = 0xfe;
    static constexpr uint8_t SIZE = 4;

private:
    Color color;

public:
    RGB(InputStream& in);
    RGB(const Color& color);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    const Color& get_color() const { return color; }

    bool operator==(const RGB& rhs) const;
};

class RGBA final : public Chunk
{
public:
    static constexpr uint8_t TAG = 0xff;
    static constexpr uint8_t SIZE = 5;

private:
    Color color;

public:
    RGBA(InputStream& in);
    RGBA(const Color& color);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    const Color& get_color() const { return color; }

    bool operator==(const RGBA& rhs) const;
};

class Luma final : public Chunk
{
public:
    static constexpr uint8_t TAG = 0x80;
    static constexpr uint8_t SIZE = 2;

private:
    uint8_t diff_green, diff_red_green, diff_blue_green;

public:
    Luma(InputStream& in);
    Luma(int8_t diff_green, int8_t diff_red_green, int8_t diff_blue_green);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    int8_t get_diff_green() const { return diff_green - 32; }
    int8_t get_diff_red_green() const { return diff_red_green - 8; }
    int8_t get_diff_blue_green() const { return diff_blue_green - 8; }

    bool operator==(const Luma& rhs) const;
};

class Run final : public Chunk
{
public:
    static constexpr uint8_t TAG = 0xc0;
    static constexpr uint8_t SIZE = 1;

private:
    uint8_t run;

public:
    Run(InputStream& in);
    Run(uint8_t run);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    uint8_t get_run() const { return run + 1; }

    bool operator==(const Run& rhs) const;
};

class Footer final : public Chunk
{
public:
    static constexpr uint8_t SIZE = 8;

public:
    Footer() {}
    Footer(InputStream& in);

    void encode(OutputStream& out) override;
    void decode(InputStream& in) override;

    bool operator==([[maybe_unused]] const Footer& rhs) const { return true; }
};

}
