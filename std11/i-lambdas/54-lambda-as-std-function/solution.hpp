#pragma once

#include <functional>
#include <vector>

/*
 * Step 54 -- lambda as std::function. See task.md.
 *
 * Implement:
 *   make_multiplier(factor) -- returns std::function<int(int)>
 *                              that multiplies by factor (captures!).
 *   apply_all(fns, x)       -- chains a vector of callables.
 */

namespace task54
{

/* TODO: return [factor](int x) { return x * factor; }; */
inline std::function<int(int)> make_multiplier(int /* factor */)
{
    return [](int n)
    { return n; };
}

/* TODO:
 *   int result = x;
 *   for (const auto &f : fns) result = f(result);
 *   return result;
 */
inline int apply_all(const std::vector<std::function<int(int)>> & /* fns */, int x)
{
    return x;
}

}
