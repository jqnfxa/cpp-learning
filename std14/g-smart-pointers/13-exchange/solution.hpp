#pragma once

#include <cstddef>
#include <utility>

/*
 * Step 13 -- std::exchange (C++14). See task.md.
 *
 * `std::exchange(obj, new_value)` writes `new_value` to `obj`
 * and returns the OLD value. The canonical use is in move
 * constructors:
 *
 *   ptr_(std::exchange(other.ptr_, nullptr))
 *
 * "Take other's pointer; leave nullptr behind."
 *
 * Two demos:
 *   1. counter::pop / counter::swap -- the basic "return and
 *      reset" / "return and replace" patterns.
 *   2. buffer's move-ctor and move-assignment -- the
 *      raw-pointer-owning class where exchange is most useful.
 *
 * The scaffold ships with broken implementations that forget
 * to reset one of the source fields. Tests detect the leftover
 * state.
 */

namespace task13
{

class counter
{
    int value_;

public:
    explicit counter(int initial) noexcept
        : value_(initial)
    {
    }

    int peek() const noexcept
    {
        return value_;
    }

    /* TODO: return std::exchange(value_, 0).
     * Stub returns value_ without resetting. */
    int pop() noexcept
    {
        return value_;
    }

    /* TODO: return std::exchange(value_, new_value).
     * Stub returns value_ without storing new_value. */
    int swap(int /*new_value*/) noexcept
    {
        return value_;
    }
};

class buffer
{
    int *data_;
    std::size_t size_;

public:
    explicit buffer(std::size_t n)
        : data_(new int[n]{})
        , size_(n)
    {
    }

    ~buffer()
    {
        delete[] data_;
    }

    buffer(const buffer &) = delete;
    buffer &operator=(const buffer &) = delete;

    /* TODO: use std::exchange for both fields:
     *   : data_(std::exchange(other.data_, nullptr))
     *   , size_(std::exchange(other.size_, 0))
     * Stub nulls data_ but forgets to reset size_. */
    buffer(buffer &&other) noexcept
        : data_(other.data_)
        , size_(other.size_)
    {
        other.data_ = nullptr;
        /* STUB: forgets `other.size_ = 0;` */
    }

    /* TODO: use std::exchange for both fields here too.
     * Stub: same omission as the move ctor. */
    buffer &operator=(buffer &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            /* STUB: forgets `other.size_ = 0;` */
        }
        return *this;
    }

    int *data() const noexcept
    {
        return data_;
    }

    std::size_t size() const noexcept
    {
        return size_;
    }
};

}
