set(HEADERS
    bits.hpp
    decoder.hpp
    exceptions.hpp
    format.hpp
    log.hpp
    pixel.hpp
    raw_image.hpp
    stream.hpp
)

set(SOURCES
    pixel.cpp
    raw_image.cpp
    stream.cpp
)

add_library(common ${HEADERS} ${SOURCES})

target_include_directories(common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
