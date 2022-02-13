#include "qoi_encoder.hpp"

#include "pixel.hpp"

void QOI::Encoder::encode()
{
    encode_header();

    Pixel previously_seen_pixels[64];
    Pixel previous_pixel;
    uint8_t run = 0;

    for (const Pixel &current_pixel : image.pixels)
    {
        if (current_pixel == previous_pixel)
        {
            ++run;
            if (run == 62)
            {
                encode_run(run);
                run = 0;
            }
        }
        else
        {
            if (run != 0)
            {
                encode_run(run);
                run = 0;
            }

            uint8_t index = hash_pixel(current_pixel);
            if (previously_seen_pixels[index] == current_pixel)
            {
                encode_index(index);
            }
            else if (current_pixel.a == previous_pixel.a)
            {
                int8_t diff_r = current_pixel.r - previous_pixel.r;
                int8_t diff_g = current_pixel.g - previous_pixel.g;
                int8_t diff_b = current_pixel.b - previous_pixel.b;

                int8_t diff_r_g = diff_r - diff_g;
                int8_t diff_b_g = diff_b - diff_g;

                if (-2 <= diff_r && diff_r <= 1 &&
                    -2 <= diff_g && diff_g <= 1 &&
                    -2 <= diff_b && diff_b <= 1)
                {
                    encode_diff(diff_r, diff_g, diff_b);
                }
                else if (
                    -8 <= diff_r_g && diff_r_g <= 7 &&
                    -32 <= diff_g && diff_g <= 31 &&
                    -8 <= diff_b_g && diff_b_g <= 7)
                {
                    encode_luma(diff_g, diff_r_g, diff_b_g);
                }
                else
                {
                    encode_rgb(current_pixel);
                }
            }
            else
            {
                encode_rgba(current_pixel);
            }
            previously_seen_pixels[index] = current_pixel;
            previous_pixel = current_pixel;
        }
    }

    if (run != 0)
        encode_run(run);

    encode_footer();
}

void QOI::Encoder::encode_header() const
{
    out.write_u32(HEADER_MAGIC);
    out.write_u32(image.width);
    out.write_u32(image.height);
    out.write_u8(settings.channels == Channels::RGB ? 3 : 4);
    out.write_u8(settings.colorspace == Colorspace::SRGB ? 0 : 1);
}

void QOI::Encoder::encode_index(uint8_t index) const
{
    out.write_u8(INDEX_TAG | index);
}

void QOI::Encoder::encode_diff(int8_t diff_r, int8_t diff_g, int8_t diff_b) const
{
    uint8_t value = (diff_r + 2) << 4 | (diff_g + 2) << 2 | (diff_b + 2);
    out.write_u8(DIFF_TAG | (value & 0x3f));
}

void QOI::Encoder::encode_luma(int8_t diff_g, int8_t diff_r_g, int8_t diff_b_g) const
{
    out.write_u8(LUMA_TAG | (diff_g + 32));
    out.write_u8((diff_r_g + 8) << 4 | (diff_b_g + 8));
}

void QOI::Encoder::encode_rgb(const Pixel &pixel) const
{
    out.write_u8(RGB_TAG);
    out.write_u8(pixel.r);
    out.write_u8(pixel.g);
    out.write_u8(pixel.b);
}

void QOI::Encoder::encode_rgba(const Pixel &pixel) const
{
    out.write_u8(RGBA_TAG);
    out.write_u8(pixel.r);
    out.write_u8(pixel.g);
    out.write_u8(pixel.b);
    out.write_u8(pixel.a);
}

void QOI::Encoder::encode_run(uint8_t run) const
{
    out.write_u8(RUN_TAG | (run - 1));
}

void QOI::Encoder::encode_footer() const
{
    out.write_u32(0x00000000);
    out.write_u32(0x00000001);
}
