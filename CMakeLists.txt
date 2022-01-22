cmake_minimum_required(VERSION 3.3)

project(lib-image-cxx CXX)
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

# -- GoogleTest
enable_testing()
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# --

add_subdirectory(src)
add_subdirectory(tests)
