#include "image_widget_bmp.hpp"

#include "bmp_decoder.hpp"

Viewer::BMPImageWidget::BMPImageWidget(InputStream& in, const std::string& filename) :
    ImageWidget { filename }
{
    BMP::Decoder { in, raw_image }.decode();
}
