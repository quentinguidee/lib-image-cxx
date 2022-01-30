set(HEADERS
    image_widget.hpp
    menu_bar.hpp
    app.hpp
)

set(SOURCES
    image_widget.cpp
    main.cpp
    menu_bar.cpp
    app.cpp
)

# -- SDL2
FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-2.0.20.zip
)
FetchContent_MakeAvailable(SDL2)
# --

# -- ImGUI
FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.86.zip
)
FetchContent_MakeAvailable(imgui)
# --

include_directories(${imgui_SOURCE_DIR})

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(viewer ${IMGUI_SOURCES} ${HEADERS} ${SOURCES})
find_package(OpenGL REQUIRED)
target_link_libraries(viewer
    image
    SDL2::SDL2
    ${OPENGL_gl_LIBRARY}
)
