#pragma once

/*
 * Step 02 -- long long. See task.md for the spec.
 *
 * Implement task02::mul64 so it returns a * b as a 64-bit integer,
 * correctly even when the product would overflow int. Do not change
 * the function signature.
 */

namespace task02
{

inline long long mul64(int a, int b)
{
    return static_cast<long long>(a) * 
           static_cast<long long>(b);
}

}
