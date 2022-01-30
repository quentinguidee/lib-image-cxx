set(SOURCES
    common_tests.cpp
    qoi_tests.cpp
)

add_executable(tests ${SOURCES})

target_link_libraries(tests
    image
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_subdirectory(test_images)
