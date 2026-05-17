#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

#include "solution.hpp"

namespace
{

/* Run `fn` in a worker thread. If it completes within `timeout_ms`,
 * join and return true. If it deadlocks, detach the worker (leaks
 * the thread for the rest of the process lifetime) and return false.
 *
 * `fn` must not capture by reference any state that the test scope
 * will dispose -- use shared_ptr for shared state. */
template <class Fn>
bool run_with_deadlock_watchdog(Fn fn, int timeout_ms)
{
    auto done = std::make_shared<std::atomic<bool>>(false);
    std::thread worker([fn, done]
                       {
        fn();
        done->store(true, std::memory_order_release); });

    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
    while (!done->load(std::memory_order_acquire) && std::chrono::steady_clock::now() < deadline)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    if (done->load(std::memory_order_acquire))
    {
        worker.join();
        return true;
    }
    worker.detach();
    return false;
}

}

TEST(recursive_mutex, fresh_counter_reads_zero)
{
    task115::counter c;
    EXPECT_EQ(c.read(), 0);
}

TEST(recursive_mutex, increment_once_works_without_recursion)
{
    /* increment() doesn't trigger the recursive lock path, so both
     * stub and reference handle it. */
    task115::counter c;
    c.increment();
    EXPECT_EQ(c.read(), 1);
}

TEST(recursive_mutex, three_independent_increments)
{
    task115::counter c;
    c.increment();
    c.increment();
    c.increment();
    EXPECT_EQ(c.read(), 3);
}

TEST(recursive_mutex, increment_twice_completes)
{
    auto c = std::make_shared<task115::counter>();
    bool ok = run_with_deadlock_watchdog(
        [c]
        { c->increment_twice(); }, 500);
    ASSERT_TRUE(ok)
        << "increment_twice deadlocked -- needs std::recursive_mutex";
    EXPECT_EQ(c->read(), 2);
}

TEST(recursive_mutex, three_increment_twice_calls)
{
    auto c = std::make_shared<task115::counter>();
    bool ok = run_with_deadlock_watchdog(
        [c]
        {
            c->increment_twice();
            c->increment_twice();
            c->increment_twice();
        },
        500);
    ASSERT_TRUE(ok) << "increment_twice deadlocked";
    EXPECT_EQ(c->read(), 6);
}

TEST(recursive_mutex, mixed_calls)
{
    auto c = std::make_shared<task115::counter>();
    bool ok = run_with_deadlock_watchdog(
        [c]
        {
            c->increment();       /* +1 */
            c->increment_twice(); /* +2 */
            c->increment();       /* +1 */
        },
        500);
    ASSERT_TRUE(ok);
    EXPECT_EQ(c->read(), 4);
}
