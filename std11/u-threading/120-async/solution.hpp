#pragma once

#include <future>

/*
 * Step 120 -- std::async + launch policies. See task.md for the spec.
 *
 * Two thin wrappers around std::async: one for launch::async (run
 * immediately on a new thread), one for launch::deferred (run lazily
 * when get/wait is called).
 */

namespace task120
{

template <class F>
std::future<int> spawn_async(F fn, int x)
{
    return std::async(std::launch::async, fn, x);
}

template <class F>
std::future<int> spawn_deferred(F fn, int x)
{
    /* TODO: change std::launch::async to std::launch::deferred. */
    return std::async(std::launch::async, fn, x);
}

}
