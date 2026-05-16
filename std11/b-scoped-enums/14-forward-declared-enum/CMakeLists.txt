cmake_minimum_required(VERSION 3.16)
project(task_14_forward_declared_enum CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

# This is the first task with a separate translation unit for the
# student's implementation: solution.cpp holds the full enum
# definition and function bodies, while solution.hpp only
# forward-declares.
add_executable(tests tests.cpp solution.cpp)
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(tests)
