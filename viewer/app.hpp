#pragma once

#include <deque>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"
#include "image_widget.hpp"

namespace Viewer {

class App
{
private:
    SDL_Window* window;
    ImGuiIO* io;
    SDL_GLContext opengl_context;

    std::vector<std::string> test_files {};

    std::deque<ImageWidget> image_widgets {};

public:
    App();
    ~App();

    void run();

    void show_menu_bar();
    void show_menu_bar_open();
    void show_menu_bar_view();

    void refresh_test_images();
    void open_image_widget(const std::string& path);
};

}
