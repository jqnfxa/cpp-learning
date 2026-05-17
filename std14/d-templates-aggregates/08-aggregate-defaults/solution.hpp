#pragma once

#include <string>

/*
 * Step 8 -- aggregates with default member initializers (C++14).
 * See task.md.
 *
 * In C++11, putting `= default_value` on a member disqualified
 * the struct from being an aggregate, so brace-initialization
 * `{}` / `{1}` / `{1, 2}` wouldn't work. C++14 fixed that:
 * aggregates may have default member initializers, AND brace-
 * initialisation still works -- with explicit brace values
 * winning over the defaults.
 *
 * Two aggregates: point (two ints) and user (string + int +
 * bool). The scaffold ships with WRONG default values; the
 * tests detect that via brace-init forms that should fall
 * through to the defaults.
 */

namespace task08
{

/* TODO: change the default values to 0 / 0.
 *   int x = 0;
 *   int y = 0;
 */
struct point
{
    int x = 999;
    int y = 999;
};

/* TODO: change the defaults to "anonymous" / 0 / true.
 *   std::string name = "anonymous";
 *   int age = 0;
 *   bool active = true;
 */
struct user
{
    std::string name = "WRONG";
    int age = -1;
    bool active = false;
};

}
