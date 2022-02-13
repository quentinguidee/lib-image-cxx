set(HEADERS
    png_common.hpp
    png_decoder.hpp
    png_format.hpp
    png_encoder.hpp
    png_settings.hpp
)

set(SOURCES
    png_common.cpp
    png_decoder.cpp
    png_encoder.cpp
)

add_library(png ${HEADERS} ${SOURCES})
target_include_directories(png PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(png common)

find_package(ZLIB REQUIRED)
target_link_libraries(png ZLIB::ZLIB)
