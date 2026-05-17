#include <gtest/gtest.h>

#include <mutex>

#include "solution.hpp"

TEST(unique_lock, single_thread_basic_doubling)
{
    int shared = 5;
    std::mutex m;
    /* Work doesn't touch the mutex; both stub and reference give 10. */
    int result = task114::snapshot_then_compute(shared, m,
                                                [](int snap)
                                                { return snap * 2; });
    EXPECT_EQ(result, 10);
    EXPECT_EQ(shared, 10);
}

TEST(unique_lock, mutex_is_released_during_work)
{
    /* Headline test. The work callable tries to acquire the mutex
     * via try_lock. If the outer snapshot_then_compute properly
     * unlocked partway, try_lock succeeds and the work returns
     * snap * 2. If the outer kept the lock held, try_lock fails
     * and the work returns -1. */
    int shared = 5;
    std::mutex m;
    int result = task114::snapshot_then_compute(shared, m, [&](int snap)
                                                {
        std::unique_lock<std::mutex> inner(m, std::try_to_lock);
        if (inner.owns_lock())
        {
            return snap * 2;
        }
        return -1; });
    EXPECT_EQ(result, 10) << "outer must have released the mutex during work";
    EXPECT_EQ(shared, 10);
}

TEST(unique_lock, mutex_is_held_for_writeback)
{
    /* Inverse of the previous: by the time control returns, the
     * mutex must be released by the outer (its destructor unlocked).
     * We verify by acquiring it from this thread post-call. */
    int shared = 3;
    std::mutex m;
    task114::snapshot_then_compute(shared, m, [](int s)
                                   { return s + 1; });
    /* After the function returns, the outer unique_lock destructed
     * and released. We can now lock m from the test thread. */
    std::unique_lock<std::mutex> outer(m, std::try_to_lock);
    EXPECT_TRUE(outer.owns_lock())
        << "outer unique_lock must release on function return";
}

TEST(unique_lock, work_can_use_mutex_for_safe_logging)
{
    /* A more realistic scenario: work() acquires the mutex briefly
     * to update a shared log, then returns. Use try_to_lock so the
     * stub's "outer didn't unlock" failure mode is a clean fail
     * rather than a deadlock hang. */
    int shared = 10;
    std::mutex m;
    int log_entries = 0;
    int result = task114::snapshot_then_compute(shared, m, [&](int snap)
                                                {
        std::unique_lock<std::mutex> g(m, std::try_to_lock);
        if (g.owns_lock())
        {
            ++log_entries;
        }
        return snap * 3; });
    EXPECT_EQ(result, 30);
    EXPECT_EQ(shared, 30);
    EXPECT_EQ(log_entries, 1)
        << "work() should have been able to acquire the mutex";
}

TEST(unique_lock, work_returning_negative)
{
    int shared = 7;
    std::mutex m;
    int result = task114::snapshot_then_compute(shared, m,
                                                [](int snap)
                                                { return -snap; });
    EXPECT_EQ(result, -7);
    EXPECT_EQ(shared, -7);
}

TEST(unique_lock, identity_work)
{
    int shared = 42;
    std::mutex m;
    int result = task114::snapshot_then_compute(shared, m,
                                                [](int snap)
                                                { return snap; });
    EXPECT_EQ(result, 42);
    EXPECT_EQ(shared, 42);
}
