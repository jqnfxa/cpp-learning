#include <gtest/gtest.h>

#include "solution.hpp"

TEST(atomic, four_threads_hundred_k)
{
    EXPECT_EQ(task117::sum_threads(4, 100000), 400000);
}

TEST(atomic, one_thread)
{
    EXPECT_EQ(task117::sum_threads(1, 1000), 1000);
}

TEST(atomic, two_threads)
{
    EXPECT_EQ(task117::sum_threads(2, 50000), 100000);
}

TEST(atomic, eight_threads_high_contention)
{
    EXPECT_EQ(task117::sum_threads(8, 50000), 400000);
}

TEST(atomic, zero_threads)
{
    EXPECT_EQ(task117::sum_threads(0, 100), 0);
}

TEST(atomic, zero_iterations)
{
    EXPECT_EQ(task117::sum_threads(4, 0), 0);
}

TEST(atomic, sixteen_threads_thousand)
{
    EXPECT_EQ(task117::sum_threads(16, 1000), 16000);
}

TEST(atomic, large_total)
{
    /* 8 threads, 1M iterations -> 8M expected. Should still be
     * fast (no mutex overhead). */
    EXPECT_EQ(task117::sum_threads(8, 1000000), 8000000);
}
