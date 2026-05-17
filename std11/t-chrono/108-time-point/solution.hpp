#pragma once

#include <chrono>

/*
 * Step 108 -- std::chrono::time_point. See task.md for the spec.
 *
 * Implement measure<F>(fn): run fn once, return the elapsed time
 * as microseconds (measured with std::chrono::steady_clock).
 */

namespace task108
{

/* TODO:
 *   auto start = std::chrono::steady_clock::now();
 *   fn();
 *   auto end = std::chrono::steady_clock::now();
 *   return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
 */
template <class F>
std::chrono::microseconds measure(F /*fn*/)
{
    return std::chrono::microseconds(0);
}

}
