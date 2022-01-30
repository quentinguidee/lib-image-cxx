add_subdirectory(common)

add_subdirectory(qoi)

add_library(image INTERFACE formats.hpp)
target_include_directories(qoi PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(image
    INTERFACE
        common
        qoi
)
