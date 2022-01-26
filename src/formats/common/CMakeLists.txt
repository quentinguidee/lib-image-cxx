set(HEADERS
    stream.hpp
)

set(SOURCES
    stream.cpp
)

add_library(common ${HEADERS} ${SOURCES})

target_include_directories(common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
