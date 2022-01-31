#include "menu_bar.hpp"

#include <dirent.h>

#include <iostream>
#include <string>

#include "formats.hpp"
#include "imgui.h"

Viewer::MenuBar::MenuBar()
{
    refresh_test_images();
}

void Viewer::MenuBar::refresh_test_images()
{
    files.clear();
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
            files.push_back(std::string(file->d_name));
    }

    closedir(directory);
}

void Viewer::MenuBar::show() const
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Open"))
        show_open();
    if (ImGui::BeginMenu("View", false))
        show_view();

    ImGui::EndMainMenuBar();
}

void Viewer::MenuBar::show_open() const
{
    for (auto file : files)
        ImGui::MenuItem(file.c_str());

    ImGui::Separator();
    for (auto it = Formats::FORMATS.begin(); it != Formats::FORMATS.end(); ++it)
        ImGui::MenuItem(it->second.extensions.front().c_str());

    ImGui::EndMenu();
}

void Viewer::MenuBar::show_view() const
{
    ImGui::EndMenu();
}
