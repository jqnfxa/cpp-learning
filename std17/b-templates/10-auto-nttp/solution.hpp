#pragma once

#include <cstddef>

/*
 * Step 10 -- auto non-type template parameters (C++17). See
 * task.md.
 *
 * C++17 lets you write `template <auto N>` where N is a
 * non-type template parameter whose TYPE is deduced from the
 * argument:
 *
 *   fixed_array<5>      // N is int, value 5
 *   fixed_array<10ULL>  // N is unsigned long long, value 10
 *
 * The scaffold uses the C++14-style `template <std::size_t N>`,
 * which forces the argument to be size_t. The tests with
 * non-size_t arguments expose the difference.
 */

namespace task10
{

/* TODO: change to `template <auto N>` so the type of N is
 * deduced from the argument. */
template <std::size_t N>
struct fixed_array
{
    int data[N];

    static constexpr auto size() noexcept
    {
        return N;
    }

    static constexpr auto value_type_size() noexcept
    {
        return sizeof(N);
    }
};

/*
 * A simpler `value_type<auto V>` -- holds the value V and
 * its type. The compile-time type of V is deduced from the
 * argument.
 */
/* TODO: change to `template <auto V>` */
template <int V>
struct value_holder
{
    static constexpr auto value()
    {
        return V;
    }

    static constexpr auto value_type_size()
    {
        return sizeof(V);
    }
};

}
