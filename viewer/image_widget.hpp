#pragma once

#include "SDL_opengl.h"
#include "raw_image.hpp"

namespace Viewer {

class ImageWidget
{
private:
    GLuint texture_id;
    RawImage raw_image;

    std::string widget_title;

    static constexpr const int ZOOM_LEVELS_COUNT = 5;
    static constexpr const float ZOOM_LEVELS_VALUES[] = {0.25f, 0.5f, 1.0f, 1.5f, 2.0f};
    static constexpr const char* ZOOM_LEVELS_LABELS[] = {"x0.25", "x0.5", "x1", "x1.5", "x2"};
    int current_zoom_level_id = 0;

public:
    ImageWidget() {}
    ImageWidget(RawImage raw_image);
    ~ImageWidget();

    void initialize();
    void show();

    RawImage& get_raw_image() { return raw_image; };
    const RawImage& get_raw_image() const { return raw_image; };

private:
    void show_zoom_level_dropdown();
    void show_image();

    void generate_widget_title();
};

}
