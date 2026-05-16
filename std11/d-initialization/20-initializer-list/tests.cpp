#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

TEST(initializer_list, sum_simple)
{
    EXPECT_EQ(task20::sum({1, 2, 3}), 6);
}

TEST(initializer_list, sum_empty)
{
    EXPECT_EQ(task20::sum({}), 0);
}

TEST(initializer_list, sum_with_negatives)
{
    EXPECT_EQ(task20::sum({-5, 10, -3}), 2);
}

TEST(initializer_list, sum_single_element)
{
    EXPECT_EQ(task20::sum({42}), 42);
}

TEST(initializer_list, max_of_typical)
{
    EXPECT_EQ(task20::max_of({3, 1, 4, 1, 5, 9, 2, 6}), 9);
}

TEST(initializer_list, max_of_single_element)
{
    EXPECT_EQ(task20::max_of({42}), 42);
}

TEST(initializer_list, max_of_all_negative)
{
    EXPECT_EQ(task20::max_of({-10, -3, -7, -1, -5}), -1);
}

TEST(initializer_list, max_of_ties)
{
    EXPECT_EQ(task20::max_of({5, 5, 5}), 5);
}

TEST(initializer_list, to_vector_three_elements)
{
    auto v = task20::to_vector({10, 20, 30});
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(initializer_list, to_vector_empty)
{
    auto v = task20::to_vector({});
    EXPECT_TRUE(v.empty());
}

TEST(initializer_list, to_vector_preserves_order)
{
    auto v = task20::to_vector({3, 1, 4, 1, 5, 9});
    EXPECT_EQ(v, (std::vector<int>{3, 1, 4, 1, 5, 9}));
}
