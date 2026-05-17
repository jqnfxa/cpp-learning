#pragma once

#include <functional>

/*
 * Step 95 -- std::bind. See task.md for the spec.
 *
 * Build two std::function<int(int)>s via std::bind: one wrapping a
 * member function with a fixed `this`, one wrapping a free function
 * with the first argument pre-filled.
 */

namespace task95
{

struct Adder
{
    int base;

    int add(int x) const
    {
        return base + x;
    }
};

namespace detail
{

inline int add_ints(int a, int b)
{
    return a + b;
}

}

/* TODO:
 *   using std::placeholders::_1;
 *   return std::bind(&Adder::add, &a, _1);
 */
inline std::function<int(int)> bind_member(const Adder & /*a*/)
{
    return {};
}

/* TODO:
 *   using std::placeholders::_1;
 *   return std::bind(detail::add_ints, base, _1);
 */
inline std::function<int(int)> bind_free(int /*base*/)
{
    return {};
}

}
