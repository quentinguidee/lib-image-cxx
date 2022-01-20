enable_testing()

set(SOURCES
    qoi_tests.cpp
)

add_executable(tests ${SOURCES})

include_directories(../src)

target_link_libraries(tests qoi)
target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

add_subdirectory(test_images)
