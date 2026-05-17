#pragma once

/*
 * Step 16 -- new order-of-evaluation rules (C++17). See task.md.
 *
 * Read-only conceptual task; classify which expressions
 * have a defined evaluation order under C++17.
 *
 * Reference body: returns true for the cases that ARE now
 * sequenced; false for the cases where evaluation order
 * remains unsequenced.
 */

namespace task16
{

enum class scenario
{
    /* `a.b` -- a is evaluated before the member access.
     * Always sequenced. */
    member_access,

    /* `a->b()` -- a (lhs) is evaluated before b()
     * arguments. C++17 sequenced. */
    arrow_call,

    /* `a[b]` -- a is evaluated before b (C++17 added
     * sequencing here). */
    subscript,

    /* `a << b << c` -- arguments to `<<` are sequenced
     * left-to-right in C++17. */
    stream_chain,

    /* `f(g(), h())` -- g() and h() are sequenced relative
     * to each other? NO -- unsequenced. C++17 didn't fix
     * this; you can still leak (e.g., shared_ptr ctor
     * trap). */
    function_arg_order,

    /* `a = b` -- a (lhs) is evaluated after b (rhs).
     * C++17 sequenced. */
    assignment,

    /* `a++ + ++a` -- unsequenced. C++17 didn't change. */
    multiple_modifications,
};

/* TODO: return true if the scenario has defined evaluation
 * order under C++17, false otherwise.
 *
 * Reference body:
 *   return s == scenario::member_access
 *          || s == scenario::arrow_call
 *          || s == scenario::subscript
 *          || s == scenario::stream_chain
 *          || s == scenario::assignment;
 *
 * function_arg_order and multiple_modifications remain
 * UNSEQUENCED in C++17.
 */
constexpr bool is_sequenced(scenario /*s*/)
{
    return false;
}

}
