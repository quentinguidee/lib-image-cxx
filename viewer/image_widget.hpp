#pragma once

#include "SDL_opengl.h"
#include "raw_image.hpp"

namespace Viewer {

class ImageWidget
{
private:
    GLuint texture_id;
    RawImage raw_image;

public:
    ImageWidget() {}
    ImageWidget(RawImage raw_image);
    ~ImageWidget();

    void initialize();
    void show() const;

    RawImage& get_raw_image() { return raw_image; };
    const RawImage& get_raw_image() const { return raw_image; };
};

}
