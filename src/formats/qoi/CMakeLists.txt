set(HEADERS
    ../color.hpp
    ../stream.hpp
    chunks.hpp
    decode_exception.hpp
    image.hpp
)

set(SOURCES
    ../color.cpp
    ../stream.cpp
    chunks.cpp
    image.cpp
)

# find_package(SDL2 REQUIRED)
add_library(qoi ${HEADERS} ${SOURCES})
# target_link_libraries(qoi PRIVATE SDL2::SDL2)
