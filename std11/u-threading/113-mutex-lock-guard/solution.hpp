#pragma once

#include <mutex>
#include <thread>
#include <vector>

/*
 * Step 113 -- std::mutex + std::lock_guard. See task.md for the spec.
 *
 * Spawn n_threads workers; each increments a shared counter
 * `iterations` times. Without the lock_guard, the increments race
 * and the final count is too low.
 */

namespace task113
{

inline int sum_threads(int n_threads, int iterations)
{
    int counter = 0;
    std::mutex m;
    (void)m; /* TODO: stop ignoring this and protect the increment with it. */

    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(n_threads));

    for (int i = 0; i < n_threads; ++i)
    {
        workers.emplace_back([&]
                             {
            for (int j = 0; j < iterations; ++j)
            {
                /* TODO: wrap the next line in:
                 *   std::lock_guard<std::mutex> guard(m);
                 */
                ++counter; /* RACE without the lock_guard */
            } });
    }

    for (auto &t : workers)
    {
        t.join();
    }

    return counter;
}

}
