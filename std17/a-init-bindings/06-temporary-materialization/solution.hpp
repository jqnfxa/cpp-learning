#pragma once

/*
 * Step 6 -- temporary materialization (C++17). See task.md.
 *
 * Binding a prvalue to a const-reference EXTENDS the
 * temporary's lifetime to the scope of the reference. C++17
 * formalised this under "temporary materialization" -- the
 * prvalue is materialised into a temporary object the moment
 * a glvalue is needed for it.
 *
 *   const trace& t = trace(99);   // t's lifetime extends to end of scope
 *
 * Two demos:
 *   - destruction_count_within_scope: returns the destruction
 *     count WHILE the const-ref is in scope. With extension, 0;
 *     without, 1.
 *   - peek_through_ref: returns t.id while the ref is alive.
 *
 * The scaffold ships with stubs that DON'T bind the prvalue,
 * so the trace is destroyed immediately at the semicolon.
 */

namespace task06
{

struct trace
{
    int id;

    static int &destroyed_count()
    {
        static int n = 0;
        return n;
    }

    static void reset() noexcept
    {
        destroyed_count() = 0;
    }

    explicit trace(int i) noexcept
        : id(i)
    {
    }

    ~trace()
    {
        ++destroyed_count();
    }

    /* Non-copyable, non-movable -- so any "implicit move" would
     * be a compile error and the test is honest. */
    trace(const trace &) = delete;
    trace(trace &&) = delete;
    trace &operator=(const trace &) = delete;
    trace &operator=(trace &&) = delete;
};

/*
 * Returns the destroyed_count() BEFORE leaving the inner
 * scope. The intent: bind the prvalue trace(99) to a const-
 * reference inside the inner scope, so it's NOT destroyed
 * until the scope ends.
 *
 * Reference (lifetime-extended): returns 0.
 * Stub (no binding -- temp dies immediately): returns 1.
 */
inline int destruction_count_within_scope()
{
    trace::reset();
    int snapshot;
    {
        /* TODO: bind to a const-reference to extend lifetime:
         *   const trace& t = trace(99);
         *   (void)t;
         *
         * The stub creates the temp and discards it -- the
         * temp dies at the end of the full expression below. */
        (void)trace(99);

        snapshot = trace::destroyed_count();
    }
    (void)snapshot;
    return snapshot;
}

/*
 * Read the id through a const-reference to a prvalue.
 *
 * Reference: returns 42.
 * Stub: dereferences a dangling pointer (UB; usually returns
 * garbage in practice, but in defensive builds may crash).
 * To keep the stub well-behaved here, we return 0.
 */
inline int peek_through_ref()
{
    trace::reset();
    /* TODO:
     *   const trace& t = trace(42);
     *   return t.id;
     *
     * Stub: doesn't bind, returns 0. */
    (void)trace(42);
    return 0;
}

}
