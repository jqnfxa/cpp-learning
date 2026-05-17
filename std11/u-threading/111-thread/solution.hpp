#pragma once

#include <thread>

/*
 * Step 111 -- std::thread. See task.md for the spec.
 *
 * Spawn a worker thread that captures its own thread::id, join,
 * and return the id.
 */

namespace task111
{

/* TODO:
 *   std::thread::id captured;
 *   std::thread t([&] { captured = std::this_thread::get_id(); });
 *   t.join();
 *   return captured;
 */
inline std::thread::id run_and_get_thread_id()
{
    return std::thread::id{};
}

}
