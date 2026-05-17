#pragma once

/*
 * Step 21 -- [[maybe_unused]] (C++17). See task.md.
 *
 * Suppresses "unused-variable" / "unused-parameter" warnings
 * without changing runtime behavior.
 */

namespace task21
{

/*
 * Stub-like function that accepts an int but doesn't use it.
 * The [[maybe_unused]] attribute silences the warning.
 */
/* TODO: add [[maybe_unused]] on the parameter:
 *   inline int do_nothing(int [[maybe_unused]] x) { return 42; }
 */
inline int do_nothing(int /*x*/)
{
    return 42;
}

/*
 * Function with a debug-only variable. In release builds,
 * [[maybe_unused]] silences the warning.
 */
inline int complex_op(int a, int b)
{
    /* TODO:
     *   [[maybe_unused]] int debug_total = a + b;  // used only in asserts
     *   return a * b;
     */
    return a * b;
}

}
