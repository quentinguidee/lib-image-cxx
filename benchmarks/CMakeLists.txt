set(SOURCES
    qoi_benchmark.cpp
)

add_executable(benchmarks ${SOURCES})

target_link_libraries(benchmarks
    common
    qoi
    benchmark::benchmark
)
