#include "image_widget.hpp"

#include <algorithm>
#include <ios>
#include <iostream>
#include <string>

#include "SDL_opengl.h"
#include "imgui.h"
#include "raw_image.hpp"

Viewer::ImageWidget::ImageWidget(RawImage raw_image) :
    raw_image(raw_image)
{
}

Viewer::ImageWidget::~ImageWidget()
{
    glDeleteTextures(1, &texture_id);
}

void Viewer::ImageWidget::show() const
{
    std::string format = raw_image.format.extensions.front();
    std::transform(format.begin(), format.end(), format.begin(), ::toupper);
    std::string title = "[DECODER] " + format + " viewer";
    ImGui::Begin(title.c_str(), NULL, ImGuiWindowFlags_NoResize);
    ImGui::Image((ImTextureID)(uintptr_t)texture_id, ImVec2(raw_image.width / 3.0f, raw_image.height / 3.0f));
    ImGui::SetWindowSize(ImVec2(raw_image.width / 3.0f + 20, raw_image.height / 3.0f + 36));
    ImGui::End();
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
}
