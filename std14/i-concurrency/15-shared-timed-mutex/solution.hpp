#pragma once

#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <thread>

/*
 * Step 15 -- std::shared_timed_mutex (C++14). See task.md.
 *
 * A reader-writer mutex: many threads may hold a SHARED lock
 * simultaneously, but only one may hold an EXCLUSIVE lock at
 * a time. Writers block all readers and vice-versa.
 *
 * concurrent_map: reads acquire shared, writes acquire exclusive.
 *
 * The scaffold ships with a plain std::mutex -- correct but
 * fully serialised. The test measures peak concurrent readers;
 * with the plain mutex the peak is 1, with shared_timed_mutex
 * the peak should be > 1.
 *
 * Step 16 will refactor to use std::shared_lock as the RAII
 * helper for the shared-lock side.
 */

namespace task15
{

class concurrent_map
{
    std::map<int, int> m_;

    /* TODO: change std::mutex to std::shared_timed_mutex. */
    mutable std::mutex mu_;

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
        /* TODO: acquire mu_ in SHARED mode so multiple gets can
         * proceed concurrently:
         *   mu_.lock_shared();
         *   ...
         *   mu_.unlock_shared();
         * (Step 16 swaps in std::shared_lock for RAII.) The
         * stub uses lock_guard on the plain mutex, which is
         * exclusive -- only one get can hold it at a time. */
        std::lock_guard<std::mutex> lk(mu_);

        record_reader_entered();
        /* Hold the lock for ~20ms so concurrent readers actually
         * pile up if the lock is shared. */
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        record_reader_left();

        auto it = m_.find(key);
        return it != m_.end() ? it->second : -1;
    }

    void put(int key, int value)
    {
        /* Writer: exclusive lock. With shared_timed_mutex,
         * std::lock_guard still acquires the EXCLUSIVE lock
         * (the default for any mutex-like type). No change
         * needed here. */
        std::lock_guard<std::mutex> lk(mu_);
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
