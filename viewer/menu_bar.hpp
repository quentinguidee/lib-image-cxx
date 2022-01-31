#pragma once

#include <vector>

#include "imgui.h"

namespace Viewer {

class MenuBar
{
private:
    std::vector<std::string> files;

public:
    MenuBar();

    void refresh_test_images();

    void show() const;

    void show_open() const;
    void show_view() const;
};

}
