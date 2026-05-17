#pragma once

/*
 * Step 65 -- thread_local storage. See task.md for the spec.
 *
 * Expose a per-thread int via four functions. The counter must be a
 * function-local `thread_local int`, returned by reference so callers
 * can see the address differs between threads.
 */

namespace task65
{

/* TODO: return a reference to a function-local `thread_local int n = 0;`. */
inline int &counter() noexcept
{
    static int n = 0; /* WRONG: shared across threads. */
    return n;
}

/* TODO: ++counter() and return the new value. */
inline int increment() noexcept
{
    return 0;
}

/* TODO: return the current counter value. */
inline int read() noexcept
{
    return 0;
}

/* TODO: counter() = 0. */
inline void reset() noexcept
{
}

}
