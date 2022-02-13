set(HEADERS
    conversion_common.hpp
)

set(SOURCES
    common_tests.cpp
    conversion_tests.cpp
)

add_executable(tests ${HEADERS} ${SOURCES})

target_link_libraries(tests
    image
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
