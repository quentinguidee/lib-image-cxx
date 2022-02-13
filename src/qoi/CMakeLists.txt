set(HEADERS
    qoi_common.hpp
    qoi_decoder.hpp
    qoi_format.hpp
    qoi_encoder.hpp
    qoi_settings.hpp
)

set(SOURCES
    qoi_common.cpp
    qoi_decoder.cpp
    qoi_encoder.cpp
)

add_library(qoi ${HEADERS} ${SOURCES})
target_include_directories(qoi PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(qoi common)
