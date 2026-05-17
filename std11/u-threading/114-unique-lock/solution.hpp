#pragma once

#include <mutex>

/*
 * Step 114 -- std::unique_lock with manual unlock partway. See task.md.
 *
 * Lock, snapshot, manually unlock, call work, manually re-lock,
 * write back. The headline feature: lk.unlock() inside the function
 * body so `work` can use the mutex if it needs to.
 */

namespace task114
{

template <class WorkFn>
int snapshot_then_compute(int &shared, std::mutex &m, WorkFn work)
{
    std::unique_lock<std::mutex> lk(m);
    int snapshot = shared;

    /* TODO: lk.unlock(); here, so `work` can lock the mutex itself. */

    int result = work(snapshot);

    /* TODO: lk.lock(); before writing back. */

    shared = result;
    return result;
}

}
