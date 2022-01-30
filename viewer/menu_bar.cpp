#include "menu_bar.hpp"

#include "formats.hpp"
#include "imgui.h"

void Viewer::MenuBar::show() const
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Open"))
    {
        for (auto it = Formats::FORMATS.begin(); it != Formats::FORMATS.end(); ++it)
            ImGui::MenuItem(it->second.extensions.front().c_str());

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View", false))
    {
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}
