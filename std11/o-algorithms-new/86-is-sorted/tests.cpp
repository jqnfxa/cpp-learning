#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

TEST(is_sorted, asc_basic_yes)
{
    EXPECT_TRUE(task86::is_sorted_asc({1, 2, 3, 4, 5}));
}

TEST(is_sorted, asc_basic_no)
{
    EXPECT_FALSE(task86::is_sorted_asc({1, 2, 1}));
    EXPECT_FALSE(task86::is_sorted_asc({5, 4, 3, 2, 1}));
}

TEST(is_sorted, asc_repeats_count_as_sorted)
{
    EXPECT_TRUE(task86::is_sorted_asc({2, 2, 2}));
    EXPECT_TRUE(task86::is_sorted_asc({1, 2, 2, 3}));
}

TEST(is_sorted, asc_empty_is_true)
{
    EXPECT_TRUE(task86::is_sorted_asc({}));
}

TEST(is_sorted, asc_single_element_is_true)
{
    EXPECT_TRUE(task86::is_sorted_asc({42}));
}

TEST(is_sorted, first_descent_basic)
{
    /* v[3] == 1 is the first element that breaks the ascending order. */
    EXPECT_EQ(task86::first_descent({1, 2, 3, 1, 4}), 3u);
}

TEST(is_sorted, first_descent_starts_at_index_one)
{
    /* The very first pair is wrong: 5 > 1. */
    EXPECT_EQ(task86::first_descent({5, 1, 2}), 1u);
}

TEST(is_sorted, first_descent_fully_sorted_returns_size)
{
    EXPECT_EQ(task86::first_descent({1, 2, 3, 4, 5}), 5u);
}

TEST(is_sorted, first_descent_empty_returns_zero)
{
    EXPECT_EQ(task86::first_descent({}), 0u);
}

TEST(is_sorted, first_descent_single_element_returns_one)
{
    EXPECT_EQ(task86::first_descent({7}), 1u);
}

TEST(is_sorted, first_descent_repeats_are_fine)
{
    /* No descent in {1, 2, 2, 3} -- repeats don't count under <. */
    EXPECT_EQ(task86::first_descent({1, 2, 2, 3}), 4u);
}

TEST(is_sorted, first_descent_descending_array)
{
    /* {5, 4, 3, 2, 1}: descent at index 1, since 4 < 5. */
    EXPECT_EQ(task86::first_descent({5, 4, 3, 2, 1}), 1u);
}
