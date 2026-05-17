#include <gtest/gtest.h>

#include "solution.hpp"

TEST(mutex_lock_guard, four_threads_hundred_k_iterations)
{
    /* The headline: 4 * 100,000 == 400,000. Without locking,
     * interleaved RMW operations lose updates and the result is
     * typically far less. */
    EXPECT_EQ(task113::sum_threads(4, 100000), 400000);
}

TEST(mutex_lock_guard, one_thread_no_contention)
{
    /* Single thread can't race against itself. */
    EXPECT_EQ(task113::sum_threads(1, 1000), 1000);
}

TEST(mutex_lock_guard, two_threads_smaller_load)
{
    /* Smaller contended load still has races without the lock; the
     * counter must come out exactly right. */
    EXPECT_EQ(task113::sum_threads(2, 50000), 100000);
}

TEST(mutex_lock_guard, eight_threads_high_contention)
{
    EXPECT_EQ(task113::sum_threads(8, 50000), 400000);
}

TEST(mutex_lock_guard, zero_threads)
{
    EXPECT_EQ(task113::sum_threads(0, 100), 0);
}

TEST(mutex_lock_guard, zero_iterations)
{
    EXPECT_EQ(task113::sum_threads(4, 0), 0);
}

TEST(mutex_lock_guard, sixteen_threads_thousand_iterations)
{
    /* Many threads, smaller per-thread workload -- still exact. */
    EXPECT_EQ(task113::sum_threads(16, 1000), 16000);
}
