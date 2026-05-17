#pragma once

#include <tuple>

/*
 * Step 1 -- structured bindings on arrays (C++17). See task.md.
 *
 * `auto [a, b, c] = arr;` destructures a C-style array (or
 * std::array) into named identifiers. C++17's first kind of
 * structured binding -- the array form.
 *
 * The scaffold ships with stubs that return constants. Tests
 * detect that.
 */

namespace task01
{

/*
 * Destructure a 3-element int array via `auto [a, b, c] =
 * arr;` and return the elements as a tuple.
 */
inline std::tuple<int, int, int> destructure(const int (&arr)[3])
{
    /* TODO:
     *   auto [a, b, c] = arr;
     *   return std::make_tuple(a, b, c);
     *
     * The stub returns zeros.
     */
    (void)arr;
    return std::make_tuple(0, 0, 0);
}

/*
 * Same, but for a 5-element array. The point: structured
 * binding works for any compile-time array size.
 */
inline std::tuple<int, int, int, int, int> destructure5(const int (&arr)[5])
{
    /* TODO:
     *   auto [a, b, c, d, e] = arr;
     *   return std::make_tuple(a, b, c, d, e);
     */
    (void)arr;
    return std::make_tuple(0, 0, 0, 0, 0);
}

/*
 * Return the sum of an array via structured bindings.
 */
inline int sum3(const int (&arr)[3])
{
    /* TODO:
     *   auto [a, b, c] = arr;
     *   return a + b + c;
     */
    (void)arr;
    return 0;
}

}
