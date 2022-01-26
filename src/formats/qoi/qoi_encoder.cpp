#include "qoi_encoder.hpp"

#include "qoi_chunks.hpp"

void QOI::Encoder::encode(OutputStream &out, RawImage &image) const
{
    Header header(
        image.width,
        image.height,
        image.channels,
        image.colorspace);

    header.encode(out);

    Color previously_seen_pixels[64];
    Color previous_color;
    uint8_t run_count = 0;

    for (const Pixel &current_pixel : image.pixels)
    {
        Color current_color(current_pixel.r, current_pixel.g, current_pixel.b, current_pixel.a);
        if (current_color == previous_color)
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

            uint8_t index = current_color.hash();
            if (previously_seen_pixels[index] == current_color)
            {
                Index(index).encode(out);
            }
            else if (current_color.alpha == previous_color.alpha)
            {
                int8_t diff_red = current_color.red - previous_color.red;
                int8_t diff_green = current_color.green - previous_color.green;
                int8_t diff_blue = current_color.blue - previous_color.blue;

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
                    RGB(current_color).encode(out);
                }
            }
            else
            {
                RGBA(current_color).encode(out);
            }
            previously_seen_pixels[index] = current_color;
            previous_color = current_color;
        }
    }

    if (run_count != 0)
        Run(run_count).encode(out);

    Footer().encode(out);
}
