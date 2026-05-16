#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

TEST(range_for_containers, sum_small_positive)
{
    EXPECT_EQ(task16::sum({1, 2, 3, 4, 5}), 15);
}

TEST(range_for_containers, sum_empty)
{
    EXPECT_EQ(task16::sum({}), 0);
}

TEST(range_for_containers, sum_single_element)
{
    EXPECT_EQ(task16::sum({42}), 42);
}

TEST(range_for_containers, sum_mixed_signs)
{
    EXPECT_EQ(task16::sum({-3, 5, -2, 4}), 4);
}

TEST(range_for_containers, sum_large)
{
    std::vector<int> v;
    for (int i = 1; i <= 100; ++i)
    {
        v.push_back(i);
    }
    EXPECT_EQ(task16::sum(v), 5050);
}

TEST(range_for_containers, join_three_strings_with_comma)
{
    EXPECT_EQ(task16::join({"a", "b", "c"}, ","), "a,b,c");
}

TEST(range_for_containers, join_empty_vector)
{
    EXPECT_EQ(task16::join({}, ","), "");
}

TEST(range_for_containers, join_single_element_has_no_separator)
{
    EXPECT_EQ(task16::join({"only"}, ","), "only");
}

TEST(range_for_containers, join_with_multichar_separator)
{
    EXPECT_EQ(task16::join({"x", "y", "z"}, " -- "), "x -- y -- z");
}

TEST(range_for_containers, join_with_empty_separator)
{
    EXPECT_EQ(task16::join({"a", "b", "c"}, ""), "abc");
}

TEST(range_for_containers, join_with_empty_strings_in_input)
{
    EXPECT_EQ(task16::join({"", "b", ""}, ","), ",b,");
}
