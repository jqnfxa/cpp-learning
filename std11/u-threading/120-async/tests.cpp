#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <future>
#include <thread>

#include "solution.hpp"

TEST(async, spawn_async_returns_correct_value)
{
    auto f = task120::spawn_async([](int x)
                                  { return x * 2; }, 5);
    EXPECT_EQ(f.get(), 10);
}

TEST(async, spawn_deferred_returns_correct_value)
{
    auto f = task120::spawn_deferred([](int x)
                                     { return x + 100; }, 5);
    EXPECT_EQ(f.get(), 105);
}

TEST(async, spawn_async_starts_immediately)
{
    std::atomic<bool> started{false};
    auto f = task120::spawn_async(
        [&](int x)
        {
            started.store(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            return x;
        },
        7);

    /* Give the worker a moment to set the flag. With launch::async,
     * it should have started by now. */
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_TRUE(started.load()) << "launch::async must start immediately";

    EXPECT_EQ(f.get(), 7);
}

TEST(async, spawn_deferred_does_not_start_until_get)
{
    /* The headline test. With launch::deferred, the lambda body must
     * NOT have run until we call f.get(). The stub uses
     * launch::async for both, so the flag becomes true during the
     * sleep -- this test catches it. */
    std::atomic<bool> started{false};
    auto f = task120::spawn_deferred(
        [&](int x)
        {
            started.store(true);
            return x;
        },
        7);

    /* Even after a generous sleep, the deferred body hasn't run. */
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    EXPECT_FALSE(started.load())
        << "launch::deferred must not run until .get() is called";

    /* Now force execution. */
    EXPECT_EQ(f.get(), 7);
    EXPECT_TRUE(started.load()) << "deferred body should have run during .get()";
}

TEST(async, spawn_deferred_runs_on_caller_thread)
{
    /* Deferred work runs on the thread that calls .get(), not on a
     * worker thread. */
    auto main_id = std::this_thread::get_id();
    std::thread::id work_id;

    auto f = task120::spawn_deferred(
        [&](int /*x*/)
        {
            work_id = std::this_thread::get_id();
            return 0;
        },
        0);

    f.get();
    EXPECT_EQ(work_id, main_id)
        << "launch::deferred runs on the caller's thread";
}

TEST(async, spawn_async_runs_on_a_different_thread)
{
    auto main_id = std::this_thread::get_id();
    std::thread::id work_id;

    auto f = task120::spawn_async(
        [&](int /*x*/)
        {
            work_id = std::this_thread::get_id();
            return 0;
        },
        0);

    f.get();
    EXPECT_NE(work_id, main_id)
        << "launch::async runs on a separate thread";
}
