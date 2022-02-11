set(HEADERS
    bmp_common.hpp
    bmp_decoder.hpp
    bmp_encoder.hpp
)

set(SOURCES
    bmp_common.cpp
    bmp_decoder.cpp
    bmp_encoder.cpp
)

add_library(bmp ${HEADERS} ${SOURCES})

target_include_directories(bmp PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(bmp common)
