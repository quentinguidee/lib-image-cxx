#include "app.hpp"

#include <dirent.h>

#include <iostream>
#include <string>
#include <vector>

#include "formats.hpp"
#include "image_widget.hpp"
#include "imgui.h"
#include "raw_image.hpp"

Viewer::App::App() :
    test_files(),
    image_widgets()
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
        for (auto it = image_widgets.begin(); it != image_widgets.end(); ++it)
            if (!it->show()) image_widgets.erase(it);

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
            test_files.push_back(std::string(file->d_name));
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
    InputFileStream in("../test_images/" + path);
    if (!in.is_open())
    {
        std::cout << "[ERROR] Couldn't open image '" << path << "'" << std::endl;
        exit(1);
    }
    std::string extension = path.substr(path.find_last_of('.') + 1);
    image_widgets.push_back(ImageWidget(RawImage()));
    image_widgets.back().get_raw_image().decode(Formats::get_by_extension(extension), in);
    image_widgets.back().initialize();
}
