#pragma once

#include <cstddef>
#include <iterator>

/*
 * Step 26 -- deprecated: std::iterator (C++17).
 *
 * Pre-C++17, you'd inherit from std::iterator<Category, T,
 * Distance, Pointer, Reference> to define an iterator type
 * cheaply. C++17 deprecated that template; expose the 5
 * typedefs directly.
 */

namespace task26
{

/*
 * A forward iterator over int values 0..N.
 * Direct typedefs (no std::iterator base).
 */
class counter_iter
{
    int v_;

public:
    /* TODO: declare the five iterator typedefs:
     *   using iterator_category = std::forward_iterator_tag;
     *   using value_type = int;
     *   using difference_type = std::ptrdiff_t;
     *   using pointer = const int*;
     *   using reference = const int&;
     *
     * Without these, std::iterator_traits<counter_iter> has
     * no usable typedefs and tests fail to compile.
     */

    explicit counter_iter(int v) noexcept
        : v_(v)
    {
    }

    int operator*() const noexcept
    {
        return v_;
    }

    counter_iter &operator++() noexcept
    {
        ++v_;
        return *this;
    }

    counter_iter operator++(int) noexcept
    {
        counter_iter tmp = *this;
        ++v_;
        return tmp;
    }

    bool operator==(const counter_iter &o) const noexcept
    {
        return v_ == o.v_;
    }

    bool operator!=(const counter_iter &o) const noexcept
    {
        return !(*this == o);
    }
};

}
