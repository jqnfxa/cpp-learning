#pragma once

#include <exception>
#include <thread>

/*
 * Step 96 -- std::exception_ptr / current_exception / rethrow_exception.
 *
 * Implement run_and_propagate<F>(fn): run fn on a worker thread,
 * capture any exception via std::current_exception, and rethrow it
 * on this thread after the worker joins.
 */

namespace task96
{

/* TODO:
 *   std::exception_ptr eptr;
 *   std::thread t([&] {
 *       try { fn(); }
 *       catch (...) { eptr = std::current_exception(); }
 *   });
 *   t.join();
 *   if (eptr) std::rethrow_exception(eptr);
 */
template <class F>
void run_and_propagate(F /*fn*/)
{
}

}
