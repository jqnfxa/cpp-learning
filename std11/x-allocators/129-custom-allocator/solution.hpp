#pragma once

#include <cstddef>
#include <memory>
#include <new>
#include <vector>

/*
 * Step 129 -- custom C++11 allocator (minimal form). See task.md
 * for the spec.
 *
 * `counting_allocator<T>` should bump global counters in
 * `stats()` on every allocate/deallocate. The scaffold
 * implements the SHAPE of an allocator but FORGETS to bump the
 * counters -- the tests detect that.
 */

namespace task129
{

struct alloc_stats
{
    std::size_t allocations;
    std::size_t deallocations;
    std::size_t bytes_allocated;
};

inline alloc_stats &stats()
{
    static alloc_stats s = {0, 0, 0};
    return s;
}

inline void reset_stats()
{
    alloc_stats &s = stats();
    s.allocations = 0;
    s.deallocations = 0;
    s.bytes_allocated = 0;
}

template <typename T>
struct counting_allocator
{
    using value_type = T;

    counting_allocator() noexcept
    {
    }

    template <typename U>
    counting_allocator(const counting_allocator<U> &) noexcept
    {
    }

    T *allocate(std::size_t n)
    {
        /* TODO: bump stats().allocations and stats().bytes_allocated.
         *   ++stats().allocations;
         *   stats().bytes_allocated += n * sizeof(T);
         */
        if (n > static_cast<std::size_t>(-1) / sizeof(T))
        {
            throw std::bad_alloc();
        }
        return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    void deallocate(T *p, std::size_t /*n*/) noexcept
    {
        /* TODO: bump stats().deallocations.
         *   ++stats().deallocations;
         */
        ::operator delete(p);
    }
};

template <typename A, typename B>
bool operator==(const counting_allocator<A> &,
                const counting_allocator<B> &) noexcept
{
    return true;
}

template <typename A, typename B>
bool operator!=(const counting_allocator<A> &,
                const counting_allocator<B> &) noexcept
{
    return false;
}

using ivec = std::vector<int, counting_allocator<int>>;

}
