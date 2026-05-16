#pragma once

#include <cstddef>
#include <memory>
#include <utility>

/*
 * Step 33 -- move assignment. See task.md for the spec.
 *
 * buffer's move constructor is already implemented (from step 32).
 * Implement the move-assignment operator with a self-move guard.
 */

namespace task33
{

class buffer
{
public:
    buffer()
        : data_(nullptr), size_(0)
    {
    }

    explicit buffer(std::size_t n)
        : data_(new int[n]()), size_(n)
    {
    }

    buffer(buffer &&other) noexcept
        : data_(std::move(other.data_)), size_(other.size_)
    {
        other.size_ = 0;
    }

    /* TODO: implement move-assignment.
     * - Self-check: `if (this != &other) { ... }`.
     * - data_ = std::move(other.data_);
     * - size_ = other.size_;
     * - other.size_ = 0;
     * - return *this;
     */
    buffer &operator=(buffer && /* other */) noexcept
    {
        return *this;
    }

    buffer(const buffer &) = delete;
    buffer &operator=(const buffer &) = delete;

    int *data()
    {
        return data_.get();
    }

    const int *data() const
    {
        return data_.get();
    }

    std::size_t size() const
    {
        return size_;
    }

private:
    std::unique_ptr<int[]> data_;
    std::size_t size_ = 0;
};

}
