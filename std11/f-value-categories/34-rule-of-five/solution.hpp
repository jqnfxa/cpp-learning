#pragma once

#include <utility>

/*
 * Step 34 -- rule of five. See task.md for the spec.
 *
 * handle owns a raw int * via new/delete. The destructor, default
 * ctor, value ctor, and accessors are provided. Write the four
 * special members: copy ctor, copy assign, move ctor, move assign.
 */

namespace task34
{

class handle
{
public:
    handle() noexcept
        : data_(nullptr)
    {
    }

    explicit handle(int v)
        : data_(new int(v))
    {
    }

    ~handle()
    {
        delete data_;
    }

    /* TODO: copy constructor -- deep copy of *other.data_ (or null). */
    handle(const handle & /* other */)
    {
    }

    /* TODO: copy assignment with self-assign guard. */
    handle &operator=(const handle & /* other */)
    {
        return *this;
    }

    /* TODO: move constructor -- steal pointer, null the source. */
    handle(handle && /* other */) noexcept
    {
    }

    /* TODO: move assignment with self-move guard. */
    handle &operator=(handle && /* other */) noexcept
    {
        return *this;
    }

    bool has_value() const
    {
        return data_ != nullptr;
    }

    int value() const
    {
        return *data_;
    }

private:
    int *data_ = nullptr;
};

}
