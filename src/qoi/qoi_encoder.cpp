#include "qoi_encoder.hpp"

#include "pixel.hpp"
#include "qoi_chunks.hpp"

void QOI::Encoder::encode(OutputStream &out, RawImage &image) const
{
    Header header(
        image.width,
        image.height,
        image.channels,
        image.colorspace);

    header.encode(out);

    Pixel previously_seen_pixels[64];
    Pixel previous_pixel;
    uint8_t run_count = 0;

    for (const Pixel &current_pixel : image.pixels)
    {
        if (current_pixel == previous_pixel)
        {
            ++run_count;
            if (run_count == 62)
            {
                Run(run_count).encode(out);
                run_count = 0;
            }
        }
        else
        {
            if (run_count != 0)
            {
                Run(run_count).encode(out);
                run_count = 0;
            }

            uint8_t index = hash_pixel(current_pixel);
            if (previously_seen_pixels[index] == current_pixel)
            {
                Index(index).encode(out);
            }
            else if (current_pixel.a == previous_pixel.a)
            {
                int8_t diff_red = current_pixel.r - previous_pixel.r;
                int8_t diff_green = current_pixel.g - previous_pixel.g;
                int8_t diff_blue = current_pixel.b - previous_pixel.b;

                int8_t diff_red_green = diff_red - diff_green;
                int8_t diff_blue_green = diff_blue - diff_green;

                if (-2 <= diff_red && diff_red <= 1 &&
                    -2 <= diff_green && diff_green <= 1 &&
                    -2 <= diff_blue && diff_blue <= 1)
                {
                    Diff(diff_red, diff_green, diff_blue).encode(out);
                }
                else if (-8 <= diff_red_green && diff_red_green <= 7 &&
                         -32 <= diff_green && diff_green <= 31 &&
                         -8 <= diff_blue_green && diff_blue_green <= 7)
                {
                    Luma(diff_green, diff_red_green, diff_blue_green).encode(out);
                }
                else
                {
                    RGB(current_pixel).encode(out);
                }
            }
            else
            {
                RGBA(current_pixel).encode(out);
            }
            previously_seen_pixels[index] = current_pixel;
            previous_pixel = current_pixel;
        }
    }

    if (run_count != 0)
        Run(run_count).encode(out);

    Footer().encode(out);
}
