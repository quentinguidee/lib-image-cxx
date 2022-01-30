#include "menu_bar.hpp"

#include "imgui.h"

void Viewer::MenuBar::show() const
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Open"))
    {
        ImGui::MenuItem("QOI");
        ImGui::MenuItem("BMP");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View", false))
    {
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}
