cmake_minimum_required(VERSION 3.16)
project(task_10_noreturn CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(tests tests.cpp)

# -Werror=return-type makes the build fail when control may reach the
# end of a non-void function. Without [[noreturn]] on panic, the
# compiler thinks positive_only can fall off the end -- and this flag
# turns that warning into a hard error.
target_compile_options(tests PRIVATE -Werror=return-type)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(tests)
