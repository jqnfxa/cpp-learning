#pragma once

/*
 * Step 14 -- inline variables (C++17). See task.md.
 *
 * C++17's `inline` keyword for variables lets you define a
 * variable at namespace scope in a header WITHOUT the ODR
 * violation that the C++14 form would cause across multiple
 * TUs.
 *
 * The scaffold uses `static` instead of `inline` -- in this
 * single-TU test setup, both compile and work, but the
 * semantics differ.
 */

namespace task14
{

/* TODO: change `static` to `inline` so the variable has
 * external linkage and a single definition across all TUs:
 *
 *   inline int global_counter = 0;
 *   inline const int max_retries = 3;
 *   inline double pi_approx = 3.14159;
 */
static int global_counter = 0;
static const int max_retries = 3;
static double pi_approx = 3.14159;

inline int increment_counter()
{
    return ++global_counter;
}

inline int read_counter()
{
    return global_counter;
}

inline void reset_counter()
{
    global_counter = 0;
}

/* These are defined in helper.cpp -- a separate TU. The
 * test exercises whether they see the SAME global_counter
 * as the inline functions above. */
int helper_increment_counter();
int helper_read_counter();

}
