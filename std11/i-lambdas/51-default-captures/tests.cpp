#include <gtest/gtest.h>

#include "solution.hpp"

TEST(default_captures, snapshot_sum_returns_60)
{
    EXPECT_EQ(task51::snapshot_sum(), 60);
}

TEST(default_captures, live_sum_returns_600)
{
    EXPECT_EQ(task51::live_sum(), 600);
}

TEST(default_captures, snapshot_is_stable_across_calls)
{
    EXPECT_EQ(task51::snapshot_sum(), task51::snapshot_sum());
}

TEST(default_captures, live_is_stable_across_calls)
{
    EXPECT_EQ(task51::live_sum(), task51::live_sum());
}

TEST(default_captures, the_two_disagree)
{
    /*
     * The whole point: with the same starting locals (10, 20, 30)
     * and the same mutations (100, 200, 300), the two functions
     * return different values because of how they captured.
     */
    EXPECT_NE(task51::snapshot_sum(), task51::live_sum());
    EXPECT_EQ(task51::live_sum() - task51::snapshot_sum(), 540);
}
