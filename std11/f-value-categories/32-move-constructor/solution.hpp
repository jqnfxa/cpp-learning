#pragma once

#include <cstddef>
#include <memory>
#include <utility>

/*
 * Step 32 -- move constructor. See task.md for the spec.
 *
 * Implement buffer's move constructor. The default ctor, sized ctor,
 * deleted copy ops, and accessors are already provided.
 */

namespace task32
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

    /* TODO: implement the move constructor.
     * - Transfer ownership of data_ from `other` via std::move.
     * - Copy size_ from `other`.
     * - Reset other.size_ to 0. */
    buffer(buffer && /* other */) noexcept
    {
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
