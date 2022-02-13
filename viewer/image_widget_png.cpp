#include "image_widget_png.hpp"

#include "png_decoder.hpp"

namespace Viewer {

PNGImageWidget::PNGImageWidget(InputStream& in, const std::string& filename) :
    ImageWidget { filename }
{
    PNG::Decoder { in, raw_image }.decode();
}

}
