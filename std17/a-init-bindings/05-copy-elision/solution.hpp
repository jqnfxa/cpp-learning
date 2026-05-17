#pragma once

#include <utility>

/*
 * Step 5 -- guaranteed copy elision (C++17). See task.md.
 *
 * C++17 guarantees that when a function returns a PRVALUE
 * (e.g. `return T(args)`), the object is constructed DIRECTLY
 * in the caller's destination -- no move, no copy. Crucially,
 * this means a function can return a NON-MOVABLE type by
 * value.
 *
 * Two demos:
 *   1. `lock_handle`: a class with copy AND move both deleted.
 *      Factory returns it by value (only legal under C++17).
 *   2. `trace_type`: counts move/copy invocations. Reference
 *      uses prvalue return (zero counts). Stub forces a move
 *      via std::move (one count).
 */

namespace task05
{

/*
 * A non-copyable, non-movable type. Used to PROVE that
 * guaranteed elision works -- no move/copy operations are
 * needed to return it from the factory.
 */
struct lock_handle
{
    int id;

    explicit lock_handle(int i)
        : id(i)
    {
    }

    lock_handle(const lock_handle &) = delete;
    lock_handle(lock_handle &&) = delete;
    lock_handle &operator=(const lock_handle &) = delete;
    lock_handle &operator=(lock_handle &&) = delete;
};

/*
 * Factory returning a non-movable type by value.
 *
 * Only legal under C++17's guaranteed copy elision. Pre-C++17
 * required at LEAST a move constructor for a return-by-value.
 *
 * TODO body: `return lock_handle(id);` -- a prvalue, directly
 * constructed in the caller's storage. The stub returns a
 * lock_handle with the wrong id.
 */
inline lock_handle make_lock(int id)
{
    /* TODO: return lock_handle(id); */
    return lock_handle(-1); /* stub: wrong id */
}

/*
 * Type that traces every move and copy via global counters.
 */
struct trace_type
{
    int value;

    explicit trace_type(int v) noexcept
        : value(v)
    {
    }

    trace_type(const trace_type &o) noexcept
        : value(o.value)
    {
        ++copy_count();
    }

    trace_type(trace_type &&o) noexcept
        : value(o.value)
    {
        ++move_count();
    }

    trace_type &operator=(const trace_type &o) noexcept
    {
        value = o.value;
        ++copy_count();
        return *this;
    }

    trace_type &operator=(trace_type &&o) noexcept
    {
        value = o.value;
        ++move_count();
        return *this;
    }

    static int &move_count()
    {
        static int n = 0;
        return n;
    }

    static int &copy_count()
    {
        static int n = 0;
        return n;
    }

    static void reset()
    {
        move_count() = 0;
        copy_count() = 0;
    }
};

/*
 * Reference factory: returns a PRVALUE. C++17 guarantees zero
 * moves and zero copies.
 *
 * Stub: uses a local variable and std::move on return -- one
 * forced move. The compiler cannot elide it because std::move
 * makes the return expression an xvalue, not a prvalue.
 */
inline trace_type make_traced(int v)
{
    /* TODO: return trace_type(v); */
    trace_type t(v);
    return std::move(t);
}

}
