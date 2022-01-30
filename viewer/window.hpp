#pragma once

#include "SDL.h"
#include "SDL_opengl.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"
#include "image_widget.hpp"

namespace Viewer {

class Window
{
private:
    SDL_Window* window;
    ImGuiIO* io;
    SDL_GLContext opengl_context;

    ImageWidget image_widget;

public:
    Window();
    ~Window();

    void run();
};

}
