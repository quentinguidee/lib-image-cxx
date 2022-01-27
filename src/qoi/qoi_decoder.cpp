#include "qoi_decoder.hpp"

#include "qoi_chunks.hpp"
#include "qoi_decode_exception.hpp"
#include "stream.hpp"

void QOI::Decoder::decode(InputStream &in, RawImage &image) const
{
    long size = in.size() - Header::SIZE - Footer::SIZE;

    Header header(in);

    image.width = header.get_width();
    image.height = header.get_height();
    image.channels = header.get_channels();
    image.colorspace = header.get_colorspace();

    image.pixels.clear();
    image.pixels.reserve(header.get_width() * header.get_height());

    Color previously_seen_pixels[64];
    Color previous_color;

    long i = 0;
    while (i < size)
    {
        uint8_t tag_8 = in.peek_8();
        uint8_t tag_2 = tag_8 & 0xc0;

        if (tag_8 == RGB::TAG)
        {
            previous_color = RGB(in).get_color();
            previously_seen_pixels[previous_color.hash()] = previous_color;
            image.pixels.push_back(previous_color.to_pixel());
            i += RGB::SIZE;
        }
        else if (tag_8 == RGBA::TAG)
        {
            previous_color = RGBA(in).get_color();
            previously_seen_pixels[previous_color.hash()] = previous_color;
            image.pixels.push_back(previous_color.to_pixel());
            i += RGBA::SIZE;
        }
        else if (tag_2 == Index::TAG)
        {
            uint8_t index = Index(in).get_index();
            previous_color = previously_seen_pixels[index];
            image.pixels.push_back(previous_color.to_pixel());
            i += Index::SIZE;
        }
        else if (tag_2 == Diff::TAG)
        {
            Diff diff(in);

            previous_color.r += diff.get_diff_red();
            previous_color.g += diff.get_diff_green();
            previous_color.b += diff.get_diff_blue();

            previously_seen_pixels[previous_color.hash()] = previous_color;
            image.pixels.push_back(previous_color.to_pixel());
            i += Diff::SIZE;
        }
        else if (tag_2 == Luma::TAG)
        {
            Luma luma(in);

            previous_color.r += luma.get_diff_green() + luma.get_diff_red_green();
            previous_color.g += luma.get_diff_green();
            previous_color.b += luma.get_diff_green() + luma.get_diff_blue_green();

            previously_seen_pixels[previous_color.hash()] = previous_color;
            image.pixels.push_back(previous_color.to_pixel());
            i += Luma::SIZE;
        }
        else if (tag_2 == Run::TAG)
        {
            uint8_t repeat = Run(in).get_run();
            for (uint8_t j = 0; j < repeat; ++j)
                image.pixels.push_back(previous_color.to_pixel());
            i += Run::SIZE;
        }
        else
        {
            throw DecodeException("Tag not recognized.");
        }
    }
}
