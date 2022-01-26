set(HEADERS
    qoi_chunks.hpp
    qoi_decode_exception.hpp
    qoi_image.hpp
)

set(SOURCES
    qoi_chunks.cpp
    qoi_image.cpp
)

# find_package(SDL2 REQUIRED)
add_library(qoi ${HEADERS} ${SOURCES})
# target_link_libraries(qoi PRIVATE SDL2::SDL2)

target_include_directories(qoi PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(qoi common)
