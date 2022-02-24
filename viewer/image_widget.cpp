#include "image_widget.hpp"

#include <algorithm>
#include <ios>
#include <iostream>
#include <string>

#include "SDL_opengl.h"
#include "imgui.h"
#include "log.hpp"
#include "raw_image.hpp"

namespace Viewer {

constexpr const float ImageWidget::ZOOM_LEVELS_VALUES[];
constexpr const char* ImageWidget::ZOOM_LEVELS_LABELS[];

ImageWidget::ImageWidget(const std::string& filename) :
    filename { filename }
{
}

ImageWidget::~ImageWidget()
{
    if (image_loaded)
        glDeleteTextures(1, &texture_id);
}

void ImageWidget::initialize()
{
    generate_widget_title();

    if (raw_image.pixels.size() != raw_image.width * raw_image.height)
    {
        error_message = "Couldn't display image: found " + std::to_string(raw_image.pixels.size()) + " pixels instead of " + std::to_string(raw_image.width * raw_image.height) + ".";
        return;
    }

    GLubyte* pixels = new GLubyte[raw_image.height * raw_image.width * 4];
    for (uint32_t i = 0; i < raw_image.pixels.size(); ++i)
    {
        const auto pixel = raw_image.pixels[i];
        pixels[(i * 4) + 0] = (GLubyte)pixel.r;
        pixels[(i * 4) + 1] = (GLubyte)pixel.g;
        pixels[(i * 4) + 2] = (GLubyte)pixel.b;
        pixels[(i * 4) + 3] = (GLubyte)255;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raw_image.width, raw_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] pixels;

    image_loaded = true;
}

bool ImageWidget::show()
{
    if (!opened) return false;
    if (texture_id == 0) initialize();
    if (ImGui::Begin(widget_title.c_str(), &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (image_loaded)
        {
            show_zoom_level_dropdown();
            show_image();
        }
        else
        {
            show_error();
        }
    }
    ImGui::End();
    return true;
}

void ImageWidget::show_zoom_level_dropdown()
{
    if (ImGui::BeginCombo("Zoom", ZOOM_LEVELS_LABELS[current_zoom_level_id]))
    {
        for (int i = 0; i < ZOOM_LEVELS_COUNT; ++i)
        {
            if (ImGui::Selectable(ZOOM_LEVELS_LABELS[i], i == current_zoom_level_id))
                current_zoom_level_id = i;
        }
        ImGui::EndCombo();
    }
}

void ImageWidget::show_image()
{
    float zoom = ZOOM_LEVELS_VALUES[current_zoom_level_id];
    ImGui::Image((ImTextureID)(uintptr_t)texture_id, ImVec2(raw_image.width * zoom, raw_image.height * zoom));
}

void ImageWidget::show_error()
{
    ImGui::TextColored(ImVec4(0.8f, 0.1f, 0.1f, 1.0f), "%s", error_message.c_str());
}

void ImageWidget::generate_widget_title()
{
    widget_title = "[DECODER] " + filename + " (" + std::to_string(raw_image.width) + "×" + std::to_string(raw_image.height) + ")";
}

}
