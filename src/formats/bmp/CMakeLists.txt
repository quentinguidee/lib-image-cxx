set(HEADERS
    chunks.hpp
    image.hpp
)

set(SOURCES
    chunks.cpp
    image.cpp
)

add_library(bmp ${HEADERS} ${SOURCES})
