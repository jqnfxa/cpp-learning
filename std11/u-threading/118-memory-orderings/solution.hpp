#pragma once

#include <atomic>
#include <thread>

/*
 * Step 118 -- memory orderings. See task.md for the spec.
 *
 * Producer writes payload then sets flag with release. Consumer
 * spins on flag with acquire, then reads payload.
 */

namespace task118
{

/* TODO:
 *   std::atomic<int>  data{0};
 *   std::atomic<bool> flag{false};
 *   int observed = -1;
 *
 *   std::thread consumer([&] {
 *       while (!flag.load(std::memory_order_acquire))
 *           std::this_thread::yield();
 *       observed = data.load(std::memory_order_relaxed);
 *   });
 *
 *   data.store(payload, std::memory_order_relaxed);
 *   flag.store(true,    std::memory_order_release);
 *
 *   consumer.join();
 *   return observed;
 */
inline int handoff(int /*payload*/)
{
    return 0;
}

}
