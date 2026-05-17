#include <gtest/gtest.h>

#include <array>
#include <vector>

#include "solution.hpp"

TEST(free_begin_end, vector_basic)
{
    std::vector<int> v{1, 2, 3, 4, 5};
    EXPECT_EQ(task92::to_string(v), "[1, 2, 3, 4, 5]");
}

TEST(free_begin_end, vector_empty)
{
    std::vector<int> v;
    EXPECT_EQ(task92::to_string(v), "[]");
}

TEST(free_begin_end, vector_single_element)
{
    std::vector<int> v{42};
    EXPECT_EQ(task92::to_string(v), "[42]");
}

TEST(free_begin_end, std_array_basic)
{
    std::array<int, 4> a{10, 20, 30, 40};
    EXPECT_EQ(task92::to_string(a), "[10, 20, 30, 40]");
}

TEST(free_begin_end, std_array_single)
{
    std::array<int, 1> a{99};
    EXPECT_EQ(task92::to_string(a), "[99]");
}

TEST(free_begin_end, c_array_basic)
{
    int arr[] = {7, 8, 9};
    EXPECT_EQ(task92::to_string(arr), "[7, 8, 9]");
}

TEST(free_begin_end, c_array_single)
{
    int arr[] = {-1};
    EXPECT_EQ(task92::to_string(arr), "[-1]");
}

TEST(free_begin_end, c_array_five_elements)
{
    int arr[5] = {1, 2, 3, 4, 5};
    EXPECT_EQ(task92::to_string(arr), "[1, 2, 3, 4, 5]");
}

TEST(free_begin_end, vector_negatives)
{
    std::vector<int> v{-3, -2, -1, 0, 1, 2, 3};
    EXPECT_EQ(task92::to_string(v), "[-3, -2, -1, 0, 1, 2, 3]");
}
