#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "solution.hpp"

TEST(future_promise, basic_doubling)
{
    EXPECT_EQ(task119::async_compute_int([](int x)
                                         { return x * 2; }, 5),
              10);
}

TEST(future_promise, identity)
{
    EXPECT_EQ(task119::async_compute_int([](int x)
                                         { return x; }, 42),
              42);
}

TEST(future_promise, negative_input)
{
    EXPECT_EQ(task119::async_compute_int([](int x)
                                         { return -x; }, 7),
              -7);
}

TEST(future_promise, zero_input)
{
    EXPECT_EQ(task119::async_compute_int([](int x)
                                         { return x + 100; }, 0),
              100);
}

TEST(future_promise, multiple_independent_calls)
{
    /* Each call spawns its own thread, makes its own promise/future. */
    EXPECT_EQ(task119::async_compute_int([](int x)
                                         { return x + 1; }, 1),
              2);
    EXPECT_EQ(task119::async_compute_int([](int x)
                                         { return x + 2; }, 1),
              3);
    EXPECT_EQ(task119::async_compute_int([](int x)
                                         { return x + 3; }, 1),
              4);
}

TEST(future_promise, exception_propagates_through_future)
{
    /* The worker throws; the main-thread future.get() rethrows on this
     * side, preserving the dynamic type. */
    EXPECT_THROW(
        task119::async_compute_int(
            [](int /*x*/) -> int
            { throw std::runtime_error("worker error"); }, 0),
        std::runtime_error);
}

TEST(future_promise, exception_preserves_what_string)
{
    try
    {
        task119::async_compute_int(
            [](int /*x*/) -> int
            { throw std::runtime_error("specific message"); }, 0);
        FAIL() << "expected the worker's exception to surface here";
    }
    catch (const std::runtime_error &e)
    {
        EXPECT_STREQ(e.what(), "specific message");
    }
}

TEST(future_promise, derived_exception_type_preserved)
{
    /* Throw a more specific type; the main-side catch on the derived
     * type matches (not the base). */
    try
    {
        task119::async_compute_int(
            [](int /*x*/) -> int
            { throw std::out_of_range("range bad"); }, 0);
        FAIL();
    }
    catch (const std::out_of_range &)
    {
        SUCCEED();
    }
    catch (const std::exception &)
    {
        FAIL() << "caught at too-general level";
    }
}

TEST(future_promise, large_workload_completes)
{
    /* Heavier work to exercise the synchronisation. */
    int result = task119::async_compute_int(
        [](int x)
        {
            long sum = 0;
            for (int i = 0; i <= x; ++i)
            {
                sum += i;
            }
            return static_cast<int>(sum);
        },
        1000);
    EXPECT_EQ(result, 500500);
}
