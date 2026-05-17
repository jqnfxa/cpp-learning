#include <gtest/gtest.h>

#include <future>
#include <thread>
#include <vector>

#include "solution.hpp"

namespace t = task15;

TEST(shared_timed_mutex, put_then_get)
{
    t::concurrent_map m;
    m.put(1, 42);
    EXPECT_EQ(m.get(1), 42);
}

TEST(shared_timed_mutex, get_missing_key_returns_negative)
{
    t::concurrent_map m;
    EXPECT_EQ(m.get(99), -1);
}

TEST(shared_timed_mutex, put_overwrites)
{
    t::concurrent_map m;
    m.put(1, 100);
    m.put(1, 200);
    EXPECT_EQ(m.get(1), 200);
}

TEST(shared_timed_mutex, multiple_distinct_keys)
{
    t::concurrent_map m;
    for (int i = 0; i < 10; ++i)
    {
        m.put(i, i * 7);
    }
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(m.get(i), i * 7);
    }
}

TEST(shared_timed_mutex, concurrent_readers_overlap)
{
    /* The KEY pedagogical test: with std::shared_timed_mutex +
     * shared locking, multiple gets can hold the lock at the
     * same time. The peak counter should rise above 1.
     *
     * The stub uses a plain mutex -- gets serialise, peak == 1. */
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

TEST(shared_timed_mutex, single_reader_has_peak_one)
{
    /* Sanity baseline: with one reader at a time, peak is 1. */
    t::concurrent_map m;
    m.put(1, 42);
    m.reset_stats();

    EXPECT_EQ(m.get(1), 42);
    EXPECT_EQ(m.peak_concurrent_readers(), 1);
}

TEST(shared_timed_mutex, writers_block_each_other)
{
    /* Writers are always exclusive, regardless of mutex type.
     * Sequential puts give predictable final state. */
    t::concurrent_map m;
    std::vector<std::thread> ws;
    ws.reserve(8);
    for (int i = 0; i < 8; ++i)
    {
        ws.emplace_back([&m, i] { m.put(i, i * i); });
    }
    for (auto &t : ws)
    {
        t.join();
    }
    for (int i = 0; i < 8; ++i)
    {
        EXPECT_EQ(m.get(i), i * i);
    }
}

TEST(shared_timed_mutex, mixed_read_write_safe)
{
    /* Writes and reads on the same map from different threads
     * must not corrupt data. Just verify the keys we wrote are
     * findable at the end. */
    t::concurrent_map m;
    std::vector<std::thread> threads;
    threads.reserve(8);
    for (int i = 0; i < 4; ++i)
    {
        threads.emplace_back([&m, i] { m.put(i, i + 100); });
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
        EXPECT_EQ(m.get(i), i + 100);
    }
}
