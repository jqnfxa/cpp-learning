#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

TEST(iota, range_n_basic)
{
    EXPECT_EQ(task90::range_n(10),
              (std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

TEST(iota, range_n_zero)
{
    EXPECT_EQ(task90::range_n(0), std::vector<int>{});
}

TEST(iota, range_n_one)
{
    EXPECT_EQ(task90::range_n(1), std::vector<int>{0});
}

TEST(iota, range_n_size)
{
    EXPECT_EQ(task90::range_n(100).size(), 100u);
}

TEST(iota, range_from_basic)
{
    EXPECT_EQ(task90::range_from(5, 10),
              (std::vector<int>{10, 11, 12, 13, 14}));
}

TEST(iota, range_from_negative_start)
{
    EXPECT_EQ(task90::range_from(5, -2),
              (std::vector<int>{-2, -1, 0, 1, 2}));
}

TEST(iota, range_from_zero_count)
{
    EXPECT_EQ(task90::range_from(0, 5), std::vector<int>{});
}

TEST(iota, range_from_one)
{
    EXPECT_EQ(task90::range_from(1, 42), std::vector<int>{42});
}

TEST(iota, range_n_matches_range_from_zero)
{
    /* range_n(n) and range_from(n, 0) must produce the same thing. */
    EXPECT_EQ(task90::range_n(7), task90::range_from(7, 0));
}
