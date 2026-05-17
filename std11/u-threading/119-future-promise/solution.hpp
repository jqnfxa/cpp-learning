#pragma once

#include <exception>
#include <future>
#include <thread>

/*
 * Step 119 -- std::future + std::promise. See task.md for the spec.
 *
 * Spawn a worker that computes fn(x) and fulfils a std::promise<int>;
 * main thread waits on the future and returns the value (or rethrows
 * if fn threw).
 */

namespace task119
{

/* TODO:
 *   std::promise<int> p;
 *   std::future<int>  f = p.get_future();
 *
 *   std::thread worker([&p, fn, x] {
 *       try { p.set_value(fn(x)); }
 *       catch (...) { p.set_exception(std::current_exception()); }
 *   });
 *
 *   worker.join();
 *   return f.get();
 */
template <class F>
int async_compute_int(F /*fn*/, int /*x*/)
{
    return 0;
}

}
