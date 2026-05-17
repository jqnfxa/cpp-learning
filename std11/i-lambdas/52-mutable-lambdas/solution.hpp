#pragma once

#include <functional>

/*
 * Step 52 -- mutable lambdas. See task.md.
 *
 * Implement make_counter using `[start]() mutable` to mutate the
 * by-value captured `start` across calls.
 */

namespace task52
{

/* TODO: return [start]() mutable { return ++start; }; */
inline std::function<int()> make_counter(int /* start */)
{
    return []()
    { return 0; };
}

}
