#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

TEST(auto_variables, sum_basic)
{
    EXPECT_EQ(task60::sum_via_iterator({1, 2, 3, 4, 5}), 15);
}

TEST(auto_variables, sum_empty)
{
    EXPECT_EQ(task60::sum_via_iterator({}), 0);
}

TEST(auto_variables, sum_single)
{
    EXPECT_EQ(task60::sum_via_iterator({42}), 42);
}

TEST(auto_variables, sum_negatives)
{
    EXPECT_EQ(task60::sum_via_iterator({-3, 5, -2, 4}), 4);
}

TEST(auto_variables, sum_large)
{
    std::vector<int> v;
    for (int i = 1; i <= 100; ++i)
    {
        v.push_back(i);
    }
    EXPECT_EQ(task60::sum_via_iterator(v), 5050);
}

TEST(auto_variables, find_index_found)
{
    EXPECT_EQ(task60::find_index({10, 20, 30, 20}, 20), 1u);
}

TEST(auto_variables, find_index_first_position)
{
    EXPECT_EQ(task60::find_index({99, 10, 20, 30}, 99), 0u);
}

TEST(auto_variables, find_index_last_position)
{
    EXPECT_EQ(task60::find_index({10, 20, 30, 99}, 99), 3u);
}

TEST(auto_variables, find_index_not_found)
{
    EXPECT_EQ(task60::find_index({10, 20, 30}, 99), 3u);
}

TEST(auto_variables, find_index_empty)
{
    EXPECT_EQ(task60::find_index({}, 1), 0u);
}
