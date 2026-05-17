#pragma once

/*
 * Step 9 -- new/delete elision (C++14). See task.md.
 *
 * This is a read-only conceptual task. The compiler may
 * OMIT a call to `operator new` (and the matching delete)
 * under specific conditions defined in [expr.new].
 *
 * To exercise the rules in test form, classify a few common
 * scenarios as elidable or not. The reference implementation
 * answers true/false per scenario; the stub returns false for
 * everything.
 *
 * See the bottom of this file and task.md for the actual
 * explanation.
 */

namespace task09
{

enum class scenario
{
    /* `int *p = new int(5); use(*p); delete p;` -- the address
     * never leaves the function, the allocator isn't replaced.
     * The compiler may put the int on the stack and skip the
     * allocation entirely. */
    local_new_delete_paired,

    /* `int *p = new int(5); save_for_later(p);` -- the pointer
     * escapes into something the compiler can't see. The
     * allocation must remain observable. */
    pointer_escapes_to_unknown_callee,

    /* `std::make_shared<T>(args...)` -- the implementation may
     * fuse the control-block allocation with T's storage into
     * ONE block. Standard explicitly allows this. */
    make_shared_fused_allocation,

    /* The program replaces `::operator new` with a tracking
     * version that logs every allocation. The compiler cannot
     * elide because eliding would change observable behaviour. */
    replaced_global_operator_new,

    /* `int *p = new int(5); ... [no matching delete in this
     * function] ...` -- the delete might happen elsewhere or
     * never. The compiler cannot prove the lifetime; no
     * elision. */
    new_without_matching_delete_in_scope,
};

/*
 * TODO: classify each scenario as elidable (true) or not (false).
 *
 *   local_new_delete_paired         -> true
 *   pointer_escapes_to_unknown_callee -> false
 *   make_shared_fused_allocation    -> true
 *   replaced_global_operator_new    -> false
 *   new_without_matching_delete_in_scope -> false
 *
 * Reference body:
 *   return s == scenario::local_new_delete_paired
 *          || s == scenario::make_shared_fused_allocation;
 */
constexpr bool may_elide(scenario /*s*/)
{
    return false;
}

/*
 * The conceptual answer (also reproduced in task.md):
 *
 * Under C++14 [expr.new]/10 (clarified in C++17 [expr.new]/13),
 * an implementation MAY omit a call to a replaceable global
 * allocation function. The optimisation is permitted when:
 *
 *   1. The pair of `new` and the matching `delete` is fully
 *      VISIBLE to the compiler (typically same function or a
 *      chain it can inline), AND
 *
 *   2. The address of the allocation does not ESCAPE in any
 *      way that would let user code distinguish "elided" from
 *      "not elided" (no pointer comparison, no storage stored
 *      to a global, no print, no opaque function call), AND
 *
 *   3. The global `operator new` / `operator delete` are NOT
 *      REPLACED in this program with versions that have
 *      observable side effects. (If a user supplied a
 *      tracking allocator, eliding would change what gets
 *      logged -- and the standard requires the program's
 *      observable behaviour be preserved.)
 *
 * Two common cases where elision IS allowed:
 *
 *   - Short-lived locally-scoped `new` / `delete` pairs.
 *   - `std::make_shared<T>` fusing the control block and T into
 *     a single allocation. (The standard explicitly permits
 *     this even when `new` is not strictly elided: ONE call
 *     to `operator new` instead of two.)
 *
 * The optimisation was always implementable in theory; C++14
 * codified that compilers are permitted to do it. C++17 made
 * the wording even more explicit.
 */

}
