#pragma once

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>

/*
 * Step 116 -- std::condition_variable. See task.md for the spec.
 *
 * Complete bounded_buffer: push blocks when full, pop blocks when
 * empty. The scaffold ships with non-blocking versions.
 */

namespace task116
{

class bounded_buffer
{
    std::deque<int> q_;
    std::size_t cap_;
    mutable std::mutex m_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    explicit bounded_buffer(std::size_t cap)
        : cap_(cap)
    {
    }

    void push(int v)
    {
        std::unique_lock<std::mutex> lk(m_);
        /* TODO: not_full_.wait(lk, [&]{ return q_.size() < cap_; }); */
        q_.push_back(v);
        /* TODO: not_empty_.notify_one(); */
    }

    int pop()
    {
        std::unique_lock<std::mutex> lk(m_);
        /* TODO: not_empty_.wait(lk, [&]{ return !q_.empty(); }); */
        if (q_.empty())
        {
            return -1; /* stub sentinel -- the proper version would WAIT here */
        }
        int v = q_.front();
        q_.pop_front();
        /* TODO: not_full_.notify_one(); */
        return v;
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> g(m_);
        return q_.size();
    }
};

}
