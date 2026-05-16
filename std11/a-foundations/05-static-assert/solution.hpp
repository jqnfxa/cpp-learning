#pragma once

/*
 * Step 05 -- static_assert. See task.md for the spec.
 *
 * Implement a constexpr predicate returning true iff n is a power
 * of two (1, 2, 4, ...). 0 is NOT a power of two. The function must
 * be usable inside static_assert -- i.e. evaluable at compile time.
 *
 * C++11 constexpr restriction: the body is a single return statement.
 * No locals, no if/for/while. Use the ternary operator or
 * short-circuiting logical operators.
 *
 * Do not change the function signature.
 */

namespace task05
{

constexpr bool is_power_of_two(unsigned long long n)
{
    /* TODO: return true iff n is a power of 2 */
    return false;
}

}
