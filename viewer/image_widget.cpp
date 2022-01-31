#include "image_widget.hpp"

#include <algorithm>
#include <ios>
#include <iostream>
#include <string>

#include "SDL_opengl.h"
#include "imgui.h"
#include "raw_image.hpp"

constexpr const float Viewer::ImageWidget::ZOOM_LEVELS_VALUES[];
constexpr const char* Viewer::ImageWidget::ZOOM_LEVELS_LABELS[];

Viewer::ImageWidget::ImageWidget(RawImage raw_image) :
    raw_image(raw_image)
{
}

Viewer::ImageWidget::~ImageWidget()
{
    glDeleteTextures(1, &texture_id);
}

void Viewer::ImageWidget::initialize()
{
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

    generate_widget_title();
}

void Viewer::ImageWidget::show()
{
    ImGui::Begin(widget_title.c_str(), NULL, ImGuiWindowFlags_NoResize);

    show_zoom_level_dropdown();
    show_image();

    ImGui::End();
}

void Viewer::ImageWidget::show_zoom_level_dropdown()
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

void Viewer::ImageWidget::show_image()
{
    float zoom = ZOOM_LEVELS_VALUES[current_zoom_level_id];
    ImGui::Image((ImTextureID)(uintptr_t)texture_id, ImVec2(raw_image.width * zoom, raw_image.height * zoom));
    ImGui::SetWindowSize(ImVec2(raw_image.width * zoom + 20, raw_image.height * zoom + 36 + 22));
}

void Viewer::ImageWidget::generate_widget_title()
{
    std::string format = raw_image.format.extensions.front();
    std::transform(format.begin(), format.end(), format.begin(), ::toupper);
    widget_title = "[DECODER] " + format + " viewer";
}
