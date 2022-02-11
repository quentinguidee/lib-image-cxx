add_subdirectory(common)

add_subdirectory(bmp)
add_subdirectory(qoi)

add_library(image formats.hpp formats.cpp)
target_include_directories(image PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(image
    common

    bmp
    qoi
)
