#pragma once

#include <cstddef>
#include <new>
#include <scoped_allocator>
#include <vector>

/*
 * Step 130 -- std::scoped_allocator_adaptor. See task.md for the
 * spec.
 *
 * A tagged allocator carries an int `tag`. We build a
 * vector-of-vectors and log the tag of every allocator that
 * serviced an allocation. With scoped_allocator_adaptor wrapping
 * the outer allocator, ALL allocations (outer + inner) report the
 * same tag. Without it, only the outer allocation reports the
 * passed-in tag; the inner vectors fall back to a
 * default-constructed allocator (tag=0).
 *
 * The scaffold uses the *plain* (non-scoped) outer allocator
 * type. The TODO is to switch to scoped_allocator_adaptor.
 */

namespace task130
{

struct alloc_log
{
    std::vector<int> tags;
};

inline alloc_log &log()
{
    static alloc_log l;
    return l;
}

inline void reset_log()
{
    log().tags.clear();
}

template <typename T>
struct tagged_alloc
{
    using value_type = T;
    int tag;

    tagged_alloc() noexcept
        : tag(0)
    {
    }

    explicit tagged_alloc(int t) noexcept
        : tag(t)
    {
    }

    template <typename U>
    tagged_alloc(const tagged_alloc<U> &o) noexcept
        : tag(o.tag)
    {
    }

    T *allocate(std::size_t n)
    {
        log().tags.push_back(tag);
        return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    void deallocate(T *p, std::size_t /*n*/) noexcept
    {
        ::operator delete(p);
    }
};

template <typename A, typename B>
bool operator==(const tagged_alloc<A> &a, const tagged_alloc<B> &b) noexcept
{
    return a.tag == b.tag;
}

template <typename A, typename B>
bool operator!=(const tagged_alloc<A> &a, const tagged_alloc<B> &b) noexcept
{
    return !(a == b);
}

using inner = std::vector<int, tagged_alloc<int>>;

/* TODO: replace tagged_alloc<inner> with
 *   std::scoped_allocator_adaptor<tagged_alloc<inner>>
 * so that the outer vector's allocator is automatically passed
 * down to each inner vector on construction. With the plain
 * tagged_alloc here, inner vectors get a default-constructed
 * (tag=0) allocator and you can SEE that in the log.
 */
using outer_alloc = tagged_alloc<inner>;
using outer = std::vector<inner, outer_alloc>;

/* Build a 2-row matrix using an allocator with the given tag.
 * Each row gets 1 push_back, so each inner vector triggers
 * exactly one allocation.
 *
 * Returns the recorded tag log: 1 entry for the outer reserve,
 * then 1 entry per inner row's push_back.
 */
inline std::vector<int> matrix_alloc_tags(int tag)
{
    reset_log();

    outer_alloc a(tag);
    outer m(a);
    m.reserve(2);

    m.emplace_back();
    m.back().push_back(0);

    m.emplace_back();
    m.back().push_back(1);

    return log().tags;
}

}
