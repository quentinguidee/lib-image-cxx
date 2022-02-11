#pragma once

#include "image_widget.hpp"
#include "stream.hpp"

namespace Viewer {

class QOIImageWidget : public ImageWidget
{
public:
    QOIImageWidget(InputStream& in, const std::string& filename);
};

}
