#include "image_widget_qoi.hpp"

#include "qoi_decoder.hpp"

Viewer::QOIImageWidget::QOIImageWidget(InputStream& in, const std::string& filename) :
    ImageWidget { filename }
{
    QOI::Decoder { in, raw_image }.decode();
}
