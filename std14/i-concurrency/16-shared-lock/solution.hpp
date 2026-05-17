#pragma once

#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <thread>

/*
 * Step 16 -- std::shared_lock (C++14). See task.md.
 *
 * std::shared_lock<Mutex> is the RAII wrapper for the SHARED
 * (reader) side of a shared_*mutex. It pairs with
 * std::unique_lock<Mutex> for the exclusive (writer) side.
 *
 * Same concurrent_map as step 15, refactored to use these
 * RAII helpers throughout.
 *
 * The scaffold uses std::unique_lock for BOTH readers and
 * writers -- correct behaviour but readers serialise. The
 * peak-concurrent-readers test catches it.
 */

namespace task16
{

class concurrent_map
{
    std::map<int, int> m_;
    mutable std::shared_timed_mutex mu_;

    mutable std::atomic<int> active_readers_{0};
    mutable std::atomic<int> max_readers_{0};

    void record_reader_entered() const
    {
        int now = ++active_readers_;
        int prev = max_readers_.load();
        while (now > prev
               && !max_readers_.compare_exchange_weak(prev, now))
        {
            /* loop */
        }
    }

    void record_reader_left() const
    {
        --active_readers_;
    }

public:
    int get(int key) const
    {
        /* TODO: use std::shared_lock to acquire the SHARED
         * (reader) lock:
         *
         *   std::shared_lock<std::shared_timed_mutex> lk(mu_);
         *
         * The stub uses unique_lock, which is exclusive. The
         * test for concurrent readers catches it. */
        std::unique_lock<std::shared_timed_mutex> lk(mu_);

        record_reader_entered();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        record_reader_left();

        auto it = m_.find(key);
        return it != m_.end() ? it->second : -1;
    }

    void put(int key, int value)
    {
        /* Writer: exclusive lock via std::unique_lock. */
        std::unique_lock<std::shared_timed_mutex> lk(mu_);
        m_[key] = value;
    }

    int peak_concurrent_readers() const noexcept
    {
        return max_readers_;
    }

    void reset_stats() noexcept
    {
        active_readers_ = 0;
        max_readers_ = 0;
    }
};

}
