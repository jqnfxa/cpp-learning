#pragma once

#include <atomic>
#include <thread>
#include <vector>

/*
 * Step 117 -- std::atomic<T>. See task.md for the spec.
 *
 * Same shape as step 113 (shared counter incremented by n_threads
 * workers), but the synchronisation primitive is std::atomic<int>
 * instead of mutex + lock_guard.
 */

namespace task117
{

inline int sum_threads(int n_threads, int iterations)
{
    /* TODO: change to std::atomic<int> and use fetch_add. */
    int counter = 0;

    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(n_threads));

    for (int i = 0; i < n_threads; ++i)
    {
        workers.emplace_back([&]
                             {
            for (int j = 0; j < iterations; ++j)
            {
                /* TODO: counter.fetch_add(1); */
                ++counter; /* RACE without atomic */
            } });
    }

    for (auto &t : workers)
    {
        t.join();
    }

    /* TODO: return counter.load(); */
    return counter;
}

}
