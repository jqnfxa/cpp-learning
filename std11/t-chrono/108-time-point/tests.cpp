#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "solution.hpp"

TEST(time_point, empty_function_takes_nonneg_microseconds)
{
    auto elapsed = task108::measure([] {});
    EXPECT_GE(elapsed.count(), 0);
}

TEST(time_point, sleep_50ms_measured_at_least_50ms)
{
    auto elapsed = task108::measure([]
                                    { std::this_thread::sleep_for(std::chrono::milliseconds(50)); });
    /* sleep_for guarantees at least the requested duration; the
     * scheduler may add a bit. We check the lower bound. */
    EXPECT_GE(elapsed.count(), 50 * 1000); /* 50,000 microseconds */
}

TEST(time_point, sleep_10ms_measured_at_least_10ms)
{
    auto elapsed = task108::measure([]
                                    { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
    EXPECT_GE(elapsed.count(), 10 * 1000);
}

TEST(time_point, return_type_is_microseconds)
{
    auto elapsed = task108::measure([] {});
    static_assert(std::is_same<decltype(elapsed), std::chrono::microseconds>::value,
                  "measure must return std::chrono::microseconds");
    (void)elapsed;
    SUCCEED();
}

TEST(time_point, lambda_actually_runs)
{
    /* Side-effect check: the lambda must execute, not just have its
     * timing measured. */
    int ran = 0;
    task108::measure([&]
                     { ran = 42; });
    EXPECT_EQ(ran, 42);
}

TEST(time_point, multiple_calls_independent)
{
    /* Each call measures its own lambda; no shared state. */
    int counter = 0;
    auto e1 = task108::measure([&]
                               { ++counter; });
    auto e2 = task108::measure([&]
                               { ++counter; });
    EXPECT_EQ(counter, 2);
    EXPECT_GE(e1.count(), 0);
    EXPECT_GE(e2.count(), 0);
}

TEST(time_point, sanity_short_lambda_under_one_second)
{
    /* Defensive upper bound: an empty lambda must not take a full
     * second to run. If it does, our system is hosed and the test
     * will fail correctly. */
    auto elapsed = task108::measure([] {});
    EXPECT_LT(elapsed.count(), 1000 * 1000); /* < 1 second */
}
