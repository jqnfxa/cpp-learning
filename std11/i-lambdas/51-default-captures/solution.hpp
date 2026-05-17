#pragma once

/*
 * Step 51 -- default captures. See task.md.
 *
 * snapshot_sum:  use [=] (capture all by value)
 * live_sum:      use [&] (capture all by reference)
 *
 * Each function declares three locals, makes a lambda with the
 * default capture, then mutates the locals before invoking.
 */

namespace task51
{

/* TODO: capture-all-by-value version. Use [=]. Should return 60. */
inline int snapshot_sum()
{
    int a = 10, b = 20, c = 30;
    auto sum = []()
    { return 0; };
    a = 100;
    b = 200;
    c = 300;
    return sum();
}

/* TODO: capture-all-by-reference version. Use [&]. Should return 600. */
inline int live_sum()
{
    int a = 10, b = 20, c = 30;
    auto sum = []()
    { return 0; };
    a = 100;
    b = 200;
    c = 300;
    return sum();
}

}
