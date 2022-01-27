#include "qoi_decoder.hpp"

#include "qoi_chunks.hpp"
#include "qoi_decode_exception.hpp"
#include "stream.hpp"

void QOI::Decoder::decode(InputStream &in, RawImage &image) const
{
    Header header(in);

    image.width = header.get_width();
    image.height = header.get_height();
    image.channels = header.get_channels();
    image.colorspace = header.get_colorspace();

    image.pixels.clear();
    image.pixels.reserve(header.get_width() * header.get_height());

    Color previously_seen_pixels[64];
    Color previous_color;

    uint8_t run = 0;
    long image_size = image.width * image.height;
    for (long i = 0; i < image_size; ++i)
    {
        uint8_t tag_8 = in.peek_8();
        uint8_t tag_2 = tag_8 & 0xc0;

        if (run > 0)
            --run;
        else if (tag_8 == RGB::TAG)
            previous_color = RGB(in).get_color();
        else if (tag_8 == RGBA::TAG)
            previous_color = RGBA(in).get_color();
        else if (tag_2 == Index::TAG)
            previous_color = previously_seen_pixels[Index(in).get_index()];
        else if (tag_2 == Diff::TAG)
            Diff(in).apply_to(previous_color);
        else if (tag_2 == Luma::TAG)
            Luma(in).apply_to(previous_color);
        else if (tag_2 == Run::TAG)
            run = Run(in).get_run() - 1;
        else
            throw DecodeException("Tag not recognized.");

        previously_seen_pixels[previous_color.hash()] = previous_color;
        image.pixels.push_back(previous_color.to_pixel());
    }
}
