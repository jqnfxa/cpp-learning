#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "solution.hpp"

TEST(is_heap, max_heap_basic_yes)
{
    /* 9 >= 5,4; 5 >= 2,3; 4 >= 1. */
    EXPECT_TRUE(task87::is_max_heap({9, 5, 4, 2, 3, 1}));
}

TEST(is_heap, max_heap_basic_no)
{
    /* Root violates: 1 < 5 (child). */
    EXPECT_FALSE(task87::is_max_heap({1, 5, 4}));
}

TEST(is_heap, max_heap_empty_is_true)
{
    EXPECT_TRUE(task87::is_max_heap({}));
}

TEST(is_heap, max_heap_single_element_is_true)
{
    EXPECT_TRUE(task87::is_max_heap({42}));
}

TEST(is_heap, max_heap_two_elements_yes)
{
    EXPECT_TRUE(task87::is_max_heap({5, 3}));
}

TEST(is_heap, max_heap_two_elements_no)
{
    EXPECT_FALSE(task87::is_max_heap({3, 5}));
}

TEST(is_heap, make_heap_output_is_valid)
{
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
    std::make_heap(v.begin(), v.end());
    EXPECT_TRUE(task87::is_max_heap(v));
}

TEST(is_heap, first_violator_basic)
{
    /* {9, 5, 4, 8, 3, 1}:
     *   parent v[1] = 5; child v[3] = 8 -> 8 > 5, violation at i=3. */
    EXPECT_EQ(task87::first_heap_violator({9, 5, 4, 8, 3, 1}), 3u);
}

TEST(is_heap, first_violator_at_index_one)
{
    /* {1, 5, 4}: child v[1] = 5 > parent v[0] = 1, violation at i=1. */
    EXPECT_EQ(task87::first_heap_violator({1, 5, 4}), 1u);
}

TEST(is_heap, first_violator_fully_valid_returns_size)
{
    EXPECT_EQ(task87::first_heap_violator({9, 5, 4, 2, 3, 1}), 6u);
}

TEST(is_heap, first_violator_empty_returns_zero)
{
    EXPECT_EQ(task87::first_heap_violator({}), 0u);
}

TEST(is_heap, first_violator_single_returns_one)
{
    EXPECT_EQ(task87::first_heap_violator({7}), 1u);
}

TEST(is_heap, equal_elements_count_as_heap)
{
    /* All equal -- no child strictly greater than parent under <. */
    EXPECT_TRUE(task87::is_max_heap({5, 5, 5, 5, 5}));
}

TEST(is_heap, deep_violation)
{
    /* {10, 8, 9, 4, 3, 11, 6}:
     *   parent v[2] = 9; child v[5] = 11 -> 11 > 9, violation at i=5. */
    EXPECT_EQ(task87::first_heap_violator({10, 8, 9, 4, 3, 11, 6}), 5u);
}
