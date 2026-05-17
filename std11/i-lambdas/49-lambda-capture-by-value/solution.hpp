#pragma once

#include <functional>

/*
 * Step 49 -- lambda captures by value. See task.md.
 *
 * Implement two functions returning std::function. Both must
 * use [value] / [delta] capture-by-value (NOT by-reference).
 */

namespace task49
{

/* TODO: return [value]() { return value; }; */
inline std::function<int()> make_constant(int /* value */)
{
    return []()
    { return 0; };
}

/* TODO: return [delta](int n) { return n + delta; }; */
inline std::function<int(int)> make_adder(int /* delta */)
{
    return [](int n)
    { return n; };
}

}
