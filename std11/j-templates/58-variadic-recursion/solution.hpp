#pragma once

#include <sstream>
#include <string>

/*
 * Step 58 -- variadic template recursion. See task.md.
 *
 * Two overloads:
 *   format(x)            -- base case
 *   format(x, y, ...)    -- recursive case (head + at least one more)
 *
 * Joins arguments into a single space-separated string.
 */

namespace task58
{

/* TODO: base case -- stream `x` into a stringstream and return its str(). */
template <class T>
std::string format(const T & /* x */)
{
    return "";
}

/* TODO: recursive case -- "<x> " + format(y, rest...). */
template <class T, class U, class... Rest>
std::string format(const T & /* x */, const U & /* y */, const Rest &.../* rest */)
{
    return "";
}

}
