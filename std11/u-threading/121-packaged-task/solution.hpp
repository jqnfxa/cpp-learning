#pragma once

#include <future>
#include <thread>
#include <utility>

/*
 * Step 121 -- std::packaged_task. See task.md for the spec.
 *
 * Wrap fn in a packaged_task<int(int)>, spawn a detached worker
 * that runs it with x, and return the future for the caller to wait
 * on.
 */

namespace task121
{

/* TODO:
 *   std::packaged_task<int(int)> task(fn);
 *   std::future<int> f = task.get_future();
 *   std::thread(std::move(task), x).detach();
 *   return f;
 */
template <class F>
std::future<int> enqueue_task(F /*fn*/, int /*x*/)
{
    std::promise<int> p;
    p.set_value(0);
    return p.get_future();
}

}
