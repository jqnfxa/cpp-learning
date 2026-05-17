#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "solution.hpp"

TEST(minmax, pair_basic)
{
    EXPECT_EQ(task88::minmax_pair(7, 3), (std::pair<int, int>{3, 7}));
    EXPECT_EQ(task88::minmax_pair(3, 7), (std::pair<int, int>{3, 7}));
}

TEST(minmax, pair_equal_values)
{
    EXPECT_EQ(task88::minmax_pair(5, 5), (std::pair<int, int>{5, 5}));
}

TEST(minmax, pair_negative_values)
{
    EXPECT_EQ(task88::minmax_pair(-3, -8), (std::pair<int, int>{-8, -3}));
}

TEST(minmax, list_basic)
{
    EXPECT_EQ(task88::minmax_list({4, 1, 7, 3, 9, 2}), (std::pair<int, int>{1, 9}));
}

TEST(minmax, list_single_element)
{
    EXPECT_EQ(task88::minmax_list({42}), (std::pair<int, int>{42, 42}));
}

TEST(minmax, list_two_elements)
{
    EXPECT_EQ(task88::minmax_list({10, 5}), (std::pair<int, int>{5, 10}));
    EXPECT_EQ(task88::minmax_list({5, 10}), (std::pair<int, int>{5, 10}));
}

TEST(minmax, list_negatives_and_zero)
{
    EXPECT_EQ(task88::minmax_list({-1, 0, -5, 3, -2}), (std::pair<int, int>{-5, 3}));
}

TEST(minmax, range_basic)
{
    EXPECT_EQ(task88::minmax_range({3, 1, 4, 1, 5, 9, 2, 6}),
              (std::pair<int, int>{1, 9}));
}

TEST(minmax, range_single_element)
{
    EXPECT_EQ(task88::minmax_range({7}), (std::pair<int, int>{7, 7}));
}

TEST(minmax, range_already_sorted_ascending)
{
    EXPECT_EQ(task88::minmax_range({1, 2, 3, 4, 5}), (std::pair<int, int>{1, 5}));
}

TEST(minmax, range_descending)
{
    EXPECT_EQ(task88::minmax_range({5, 4, 3, 2, 1}), (std::pair<int, int>{1, 5}));
}

TEST(minmax, range_all_equal)
{
    EXPECT_EQ(task88::minmax_range({4, 4, 4, 4}), (std::pair<int, int>{4, 4}));
}

TEST(minmax, range_negatives_and_positives)
{
    EXPECT_EQ(task88::minmax_range({-3, 5, -10, 2, 0, 8, -1}),
              (std::pair<int, int>{-10, 8}));
}
