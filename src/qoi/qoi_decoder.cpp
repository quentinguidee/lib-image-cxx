#include "qoi_decoder.hpp"

#include "pixel.hpp"
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

    Pixel previously_seen_pixels[64];
    Pixel pixel;

    uint8_t run = 0;
    long image_size = image.width * image.height;
    for (long i = 0; i < image_size; ++i)
    {
        uint8_t tag_8 = in.peek_8();
        uint8_t tag_2 = tag_8 & 0xc0;

        if (run > 0)
            --run;
        else if (tag_8 == RGB::TAG)
            pixel = RGB(in).get_pixel();
        else if (tag_8 == RGBA::TAG)
            pixel = RGBA(in).get_pixel();
        else if (tag_2 == Index::TAG)
            pixel = previously_seen_pixels[Index(in).get_index()];
        else if (tag_2 == Diff::TAG)
            Diff(in).apply_to(pixel);
        else if (tag_2 == Luma::TAG)
            Luma(in).apply_to(pixel);
        else if (tag_2 == Run::TAG)
            run = Run(in).get_run() - 1;
        else
            throw DecodeException("Tag not recognized.");

        previously_seen_pixels[hash_pixel(pixel)] = pixel;
        image.pixels.push_back(pixel);
    }
}
