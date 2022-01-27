set(HEADERS
    qoi_chunks.hpp
    qoi_decoder.hpp
    qoi_encoder.hpp
)

set(SOURCES
    qoi_chunks.cpp
    qoi_decoder.cpp
    qoi_encoder.cpp
)

# find_package(SDL2 REQUIRED)
add_library(qoi ${HEADERS} ${SOURCES})
# target_link_libraries(qoi PRIVATE SDL2::SDL2)

target_include_directories(qoi PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(qoi common)
