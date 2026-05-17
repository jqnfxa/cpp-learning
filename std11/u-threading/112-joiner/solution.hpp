#pragma once

#include <thread>
#include <utility>

/*
 * Step 112 -- joiner RAII wrapper around std::thread.
 *
 * The dtor currently DETACHES the thread. The TODO is to swap that
 * for JOIN so the destructor blocks until the thread finishes.
 */

namespace task112
{

class joiner
{
    std::thread t_;

public:
    explicit joiner(std::thread t)
        : t_(std::move(t))
    {
    }

    joiner(const joiner &) = delete;
    joiner &operator=(const joiner &) = delete;

    joiner(joiner &&) = default;
    joiner &operator=(joiner &&) = delete;

    /* TODO: replace `t_.detach()` with `t_.join()`.
     *       (The guard `if (t_.joinable())` stays either way.) */
    ~joiner()
    {
        if (t_.joinable())
        {
            t_.detach();
        }
    }

    bool joinable() const noexcept
    {
        return t_.joinable();
    }

    std::thread::id get_id() const noexcept
    {
        return t_.get_id();
    }
};

}
