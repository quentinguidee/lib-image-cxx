#pragma once

#include "image_widget.hpp"
#include "stream.hpp"

namespace Viewer {

class BMPImageWidget : public ImageWidget
{
public:
    BMPImageWidget(InputStream& in, const std::string& filename);
};

}
