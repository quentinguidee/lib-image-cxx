#pragma once

#include "image_widget.hpp"
#include "stream.hpp"

namespace Viewer {

class PNGImageWidget : public ImageWidget
{
public:
    PNGImageWidget(InputStream& in, const std::string& filename);
};

}
