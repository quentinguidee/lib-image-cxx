set(HEADERS
    common.hpp
)

set(SOURCES
    benchmarks.cpp
)

add_executable(benchmarks ${HEADERS} ${SOURCES})

target_link_libraries(benchmarks
    image
    benchmark::benchmark
)
