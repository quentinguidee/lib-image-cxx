#include "window.hpp"

#include <iostream>

#include "decoder.hpp"
#include "qoi_decoder.hpp"
#include "raw_image.hpp"

Viewer::Window::Window() :
    menu_bar()
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

    InputFileStream in("../tests/test_images/image_1.qoi");
    if (!in.is_open())
    {
        std::cout << "[ERROR] Couldn't open file" << std::endl;
        exit(1);
    }

    image_widget = ImageWidget(RawImage());
    image_widget.get_raw_image().decode(QOI::Decoder(), in);
    image_widget.initialize();
}

Viewer::Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(opengl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Viewer::Window::run()
{
    ImVec4 clear_color = ImVec4(0.08, 0.08, 0.13, 1);

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

        menu_bar.show();
        image_widget.show();

        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
}
