#pragma once

#include "SDL_opengl.h"
#include "format.hpp"
#include "raw_image.hpp"

namespace Viewer {

class ImageWidget
{
private:
    bool image_loaded { false };
    GLuint texture_id { 0 };
    std::string filename;

    bool opened { true };
    std::string widget_title {};

    std::string error_message {};

    static constexpr const int ZOOM_LEVELS_COUNT { 5 };
    static constexpr const float ZOOM_LEVELS_VALUES[] { 0.25f, 0.5f, 1.0f, 1.5f, 2.0f };
    static constexpr const char* ZOOM_LEVELS_LABELS[] { "x0.25", "x0.5", "x1", "x1.5", "x2" };
    int current_zoom_level_id { 0 };

protected:
    RawImage raw_image;

public:
    ImageWidget() {}
    ImageWidget(const std::string& filename);
    ~ImageWidget();

    void initialize();

    // Returns false if this window has been closed.
    bool show();

    RawImage& get_raw_image() { return raw_image; };
    const RawImage& get_raw_image() const { return raw_image; };

private:
    void show_zoom_level_dropdown();
    void show_image();
    void show_error();

    void generate_widget_title();
};

}
