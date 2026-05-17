cmake_minimum_required(VERSION 3.16)
project(task_54_lambda_as_std_function CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(tests)
