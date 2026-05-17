#include <gtest/gtest.h>

#include <future>
#include <thread>
#include <vector>

#include "solution.hpp"

namespace t = task16;

TEST(shared_lock, put_then_get)
{
    t::concurrent_map m;
    m.put(1, 42);
    EXPECT_EQ(m.get(1), 42);
}

TEST(shared_lock, get_missing_key)
{
    t::concurrent_map m;
    EXPECT_EQ(m.get(99), -1);
}

TEST(shared_lock, put_overwrites)
{
    t::concurrent_map m;
    m.put(1, 100);
    m.put(1, 200);
    EXPECT_EQ(m.get(1), 200);
}

TEST(shared_lock, multiple_keys)
{
    t::concurrent_map m;
    for (int i = 0; i < 10; ++i)
    {
        m.put(i, i * 11);
    }
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(m.get(i), i * 11);
    }
}

TEST(shared_lock, readers_overlap_with_shared_lock)
{
    /* The KEY test for std::shared_lock: it acquires the
     * SHARED mode of the mutex, so multiple readers can hold
     * it concurrently. peak > 1 only if shared_lock was used.
     * Stub uses unique_lock -> readers serialise -> peak == 1. */
    t::concurrent_map m;
    m.put(1, 42);
    m.reset_stats();

    std::vector<std::future<int>> readers;
    readers.reserve(4);
    for (int i = 0; i < 4; ++i)
    {
        readers.push_back(std::async(std::launch::async,
                                     [&m] { return m.get(1); }));
    }
    for (auto &f : readers)
    {
        EXPECT_EQ(f.get(), 42);
    }
    EXPECT_GT(m.peak_concurrent_readers(), 1);
}

TEST(shared_lock, single_reader_peak_is_one)
{
    t::concurrent_map m;
    m.put(1, 42);
    m.reset_stats();
    EXPECT_EQ(m.get(1), 42);
    EXPECT_EQ(m.peak_concurrent_readers(), 1);
}

TEST(shared_lock, concurrent_writers_serialise)
{
    /* unique_lock on the writer side is exclusive. Concurrent
     * puts produce a coherent final state. */
    t::concurrent_map m;
    std::vector<std::thread> ws;
    ws.reserve(8);
    for (int i = 0; i < 8; ++i)
    {
        ws.emplace_back([&m, i] { m.put(i, i + 1000); });
    }
    for (auto &t : ws)
    {
        t.join();
    }
    for (int i = 0; i < 8; ++i)
    {
        EXPECT_EQ(m.get(i), i + 1000);
    }
}

TEST(shared_lock, lock_is_raii_released_on_scope_exit)
{
    /* The whole point of using shared_lock instead of manual
     * lock_shared/unlock_shared: the lock is released when
     * `lk` goes out of scope, even if an exception is thrown
     * inside the critical section. We don't simulate an
     * exception here, but sequential put-then-get verifies
     * the lock IS released after get() returns. */
    t::concurrent_map m;
    m.put(1, 5);
    EXPECT_EQ(m.get(1), 5); /* if get() didn't release, this would deadlock */
    m.put(1, 10);           /* and so would this put */
    EXPECT_EQ(m.get(1), 10);
}

TEST(shared_lock, mixed_read_write_safe)
{
    t::concurrent_map m;
    std::vector<std::thread> threads;
    threads.reserve(8);
    for (int i = 0; i < 4; ++i)
    {
        threads.emplace_back([&m, i] { m.put(i, i * 13); });
    }
    for (int i = 0; i < 4; ++i)
    {
        threads.emplace_back([&m, i] { (void)m.get(i); });
    }
    for (auto &t : threads)
    {
        t.join();
    }
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(m.get(i), i * 13);
    }
}
