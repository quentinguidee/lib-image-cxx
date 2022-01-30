#pragma once

#include "SDL.h"
#include "SDL_opengl.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"
#include "image_widget.hpp"
#include "menu_bar.hpp"

namespace Viewer {

class App
{
private:
    SDL_Window* window;
    ImGuiIO* io;
    SDL_GLContext opengl_context;

    MenuBar menu_bar;
    ImageWidget image_widget;

public:
    App();
    ~App();

    void run();
};

}
