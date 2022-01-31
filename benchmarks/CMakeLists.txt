set(SOURCES
    qoi_benchmark.cpp
)

add_executable(benchmarks ${SOURCES})

target_link_libraries(benchmarks
    image
    benchmark::benchmark
)
