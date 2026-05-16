#pragma once

#include <cstddef>

/*
 * Step 06 -- alignof. See task.md for the spec.
 *
 * `mixed` below has char/int/char members. Standard layout rules
 * force padding so that:
 *   - `b` sits on an int boundary inside the struct, and
 *   - sizeof(mixed) is a multiple of alignof(mixed).
 *
 * Your job is to expose alignof and sizeof of various types via
 * constexpr wrappers. Do not change the function signatures or the
 * struct definition.
 */

namespace task06
{

struct mixed
{
    char a;
    int b;
    char c;
};

constexpr std::size_t alignment_of_char()
{
    /* TODO: return alignof(char) */
    return 0;
}

constexpr std::size_t alignment_of_int()
{
    /* TODO: return alignof(int) */
    return 0;
}

constexpr std::size_t alignment_of_double()
{
    /* TODO: return alignof(double) */
    return 0;
}

constexpr std::size_t alignment_of_long_double()
{
    /* TODO: return alignof(long double) */
    return 0;
}

constexpr std::size_t alignment_of_mixed()
{
    /* TODO: return alignof(mixed) */
    return 0;
}

constexpr std::size_t size_of_mixed()
{
    /* TODO: return sizeof(mixed) */
    return 0;
}

}
