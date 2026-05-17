#pragma once

#include <mutex>

/*
 * Step 115 -- std::recursive_mutex. See task.md for the spec.
 *
 * counter::increment_twice() locks the mutex, then calls increment()
 * which locks it AGAIN on the same thread. That requires
 * std::recursive_mutex; with plain std::mutex the second lock
 * deadlocks against the first.
 *
 * The scaffold uses std::mutex (and lock_guard<std::mutex>). The TODO
 * is to swap both for the recursive variants.
 */

namespace task115
{

class counter
{
    int value_ = 0;
    /* TODO: change to std::recursive_mutex. */
    mutable std::mutex m_;

public:
    void increment()
    {
        /* TODO: change to lock_guard<std::recursive_mutex>. */
        std::lock_guard<std::mutex> g(m_);
        ++value_;
    }

    void increment_twice()
    {
        /* TODO: change to lock_guard<std::recursive_mutex>. */
        std::lock_guard<std::mutex> g(m_);
        increment(); /* recursive lock attempt -- needs recursive_mutex */
        ++value_;
    }

    int read() const
    {
        /* TODO: change to lock_guard<std::recursive_mutex>. */
        std::lock_guard<std::mutex> g(m_);
        return value_;
    }
};

}
