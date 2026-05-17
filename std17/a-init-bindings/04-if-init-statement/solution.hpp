#pragma once

#include <map>
#include <string>

/*
 * Step 4 -- if and switch with init-statement (C++17). See
 * task.md.
 *
 * C++17 lets you write `if (init; cond)` and `switch (init;
 * expr)`. The initialiser runs first; the condition uses
 * names from the init. The init-declared names are scoped
 * to the if/else (or switch) bodies and disappear afterward.
 *
 * Three functions exercising the pattern. The scaffold uses
 * the old C++14 pre-declaration form, which leaks the name
 * into the enclosing scope. Tests detect via the values.
 */

namespace task04
{

/*
 * Look up key in m. Return its value if present, else
 * `default_value`.
 *
 * The C++17 form: `if (auto it = m.find(k); it != m.end())`.
 */
inline int lookup_or_default(const std::map<int, int> &m, int k, int default_value)
{
    /* TODO:
     *   if (auto it = m.find(k); it != m.end()) {
     *       return it->second;
     *   }
     *   return default_value;
     *
     * Stub returns default_value always.
     */
    (void)m;
    (void)k;
    return default_value;
}

/*
 * Look up key, return its value times 2, or 0 if missing.
 */
inline int lookup_doubled(const std::map<int, int> &m, int k)
{
    /* TODO:
     *   if (auto it = m.find(k); it != m.end()) {
     *       return it->second * 2;
     *   }
     *   return 0;
     */
    (void)m;
    (void)k;
    return 0;
}

/*
 * Classify an int via switch-with-init-statement:
 *   <= 0  -> "non-positive"
 *   1..9  -> "small"
 *   10..  -> "large"
 *
 * Demonstrates that the init-statement form also works for
 * switch, not just if.
 */
inline std::string classify(int x)
{
    /* TODO:
     *   switch (int v = x; v <= 0 ? 0 : v < 10 ? 1 : 2) {
     *       case 0: return "non-positive";
     *       case 1: return "small";
     *       case 2: return "large";
     *   }
     *   return "?";
     *
     * The init-statement variant just demonstrates that
     * `switch (init; expr)` is also legal in C++17. */
    (void)x;
    return "?";
}

}
