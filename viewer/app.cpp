#include "app.hpp"

#include <dirent.h>

#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "SDL_video.h"
#include "bmp_decoder.hpp"
#include "bmp_format.hpp"
#include "image_widget.hpp"
#include "image_widget_bmp.hpp"
#include "image_widget_qoi.hpp"
#include "imgui.h"
#include "qoi_decoder.hpp"
#include "qoi_format.hpp"
#include "raw_image.hpp"

Viewer::App::App()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("[ERROR] %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        "Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        (int)1200, (int)700, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    opengl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, opengl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = &ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, opengl_context);
    ImGui_ImplOpenGL3_Init();

    refresh_test_images();
}

Viewer::App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(opengl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Viewer::App::run()
{
    ImVec4 clear_color = ImVec4(0.05, 0.05, 0.07, 1);

    SDL_Event event;
    bool exit = false;
    while (!exit)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) exit = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        show_menu_bar();
        auto it = image_widgets.begin();
        while (it != image_widgets.end())
        {
            if (!it->show())
                it = image_widgets.erase(it);
            else
                it++;
        }

        ImGui::Render();

        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
}

void Viewer::App::refresh_test_images()
{
    test_files.clear();
    DIR* directory = opendir("../test_images");
    if (directory == nullptr)
    {
        std::cerr << "test_images directory not found" << std::endl;
        return;
    }

    struct dirent* file;
    while ((file = readdir(directory)))
    {
        if (file->d_name[0] == 'i')
            test_files.push_back(std::string { file->d_name });
    }

    closedir(directory);
}

void Viewer::App::show_menu_bar()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Open"))
        show_menu_bar_open();
    if (ImGui::BeginMenu("View", false))
        show_menu_bar_view();

    ImGui::EndMainMenuBar();
}

void Viewer::App::show_menu_bar_open()
{
    for (auto file : test_files)
        if (ImGui::MenuItem(file.c_str()))
            open_image_widget(file);

    ImGui::Separator();
    if (ImGui::MenuItem("refresh"))
        refresh_test_images();

    ImGui::EndMenu();
}

void Viewer::App::show_menu_bar_view()
{
    ImGui::EndMenu();
}

void Viewer::App::open_image_widget(const std::string& path)
{
    InputFileStream in { "../test_images/" + path };
    if (!in.is_open())
    {
        std::cout << "[ERROR] Couldn't open image '" << path << "'" << std::endl;
        exit(1);
    }

    if (QOI::Decoder::can_decode(in))
        image_widgets.push_back(std::move(QOIImageWidget { in, path }));
    else if (BMP::Decoder::can_decode(in))
        image_widgets.push_back(std::move(BMPImageWidget { in, path }));
    else
        throw std::runtime_error { "Couldn't decode this image." };
}
