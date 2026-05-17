#pragma once

/*
 * Step 50 -- lambda captures by reference. See task.md.
 *
 * Implement two functions where each uses a lambda that captures
 * a *local* variable by reference (`[&counter]` / `[&total]`).
 */

namespace task50
{

/* TODO:
 *   int counter = 0;
 *   auto inc = [&counter]() { ++counter; };
 *   for (int i = 0; i < n; ++i) inc();
 *   return counter;
 */
inline int count_up_to(int /* n */)
{
    return 0;
}

/* TODO:
 *   int total = 0;
 *   auto add = [&total](int x) { total += x; };
 *   for (int i = 0; i < n; ++i) add(arr[i]);
 *   return total;
 */
inline int sum_via_reference_capture(const int * /* arr */, int /* n */)
{
    return 0;
}

}
