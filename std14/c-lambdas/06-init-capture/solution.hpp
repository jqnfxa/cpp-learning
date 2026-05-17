#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>

/*
 * Step 6 -- init-capture (capture by move / by computed
 * expression). See task.md.
 *
 * C++14's `[name = expr]` syntax. The captured member of the
 * closure is initialized from `expr`. This is what unlocks
 * move-capture: `[p = std::move(p)]`.
 *
 * Three demos:
 *   1. make_counter -- init-capture from a value parameter.
 *   2. make_growable -- init-capture by MOVE on a string.
 *   3. hold_unique -- init-capture by MOVE on a unique_ptr
 *      (the textbook reason init-capture exists).
 *
 * The scaffold ships with bodies that ignore the input -- they
 * compile but use the WRONG initial state. Tests detect that.
 */

namespace task06
{

/*
 * Returns a lambda that yields `initial`, `initial+1`,
 * `initial+2`, ... on successive calls.
 *
 * Init-capture lets us start the counter from a runtime value.
 */
inline auto make_counter(int initial)
{
    /* TODO: change `0` to `initial`.
     *   return [count = initial]() mutable { return count++; };
     */
    (void)initial;
    return [count = 0]() mutable
    { return count++; };
}

/*
 * Returns a lambda that appends "!" to the captured string and
 * yields the new size.
 *
 * Move-capture: `[s = std::move(initial)]`. The lambda owns the
 * string outright; the caller's local is empty after the call.
 */
inline auto make_growable(std::string initial)
{
    /* TODO: capture initial by MOVE into a string `s`.
     *   return [s = std::move(initial)]() mutable {
     *       s += "!";
     *       return s.size();
     *   };
     */
    (void)initial;
    return [s = std::string{}]() mutable
    {
        s += "!";
        return s.size();
    };
}

/*
 * Move-captures a unique_ptr into a std::function. This is the
 * canonical reason init-capture exists -- C++11 lambdas could
 * only capture by copy or reference, neither of which works for
 * unique_ptr.
 *
 * The returned std::function holds the unique_ptr's storage
 * for as long as the function is alive.
 */
inline std::function<int()> hold_unique(std::unique_ptr<int> p)
{
    /* TODO: capture p by MOVE (via a shared_ptr wrapper, since
     * std::function requires copyable targets in C++14).
     *
     *   auto sp = std::shared_ptr<int>(std::move(p));
     *   return [sp = std::move(sp)]() { return *sp; };
     *
     * The stub eagerly DEREFERENCES p and copies the value --
     * the lambda holds an int, not the original storage. Tests
     * by-value happen to pass because the int matches; the
     * pedagogical point (move-capture) is lost.
     */
    int v = p ? *p : 0;
    return [v]()
    { return v; };
}

}
