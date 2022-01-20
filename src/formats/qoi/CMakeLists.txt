set(HEADERS
    chunks.hpp
    decode_exception.hpp
    image.hpp
    stream.hpp
)

set(SOURCES
    chunks.cpp
    image.cpp
    stream.cpp
)

# find_package(SDL2 REQUIRED)
add_library(qoi ${HEADERS} ${SOURCES})
# target_link_libraries(qoi PRIVATE SDL2::SDL2)
