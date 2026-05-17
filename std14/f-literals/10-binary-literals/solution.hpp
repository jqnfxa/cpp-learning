#pragma once

#include <cstdint>

/*
 * Step 10 -- binary literals (C++14). See task.md.
 *
 * C++14 added the `0b...` prefix for binary integer literals.
 * `0b1010` is 10, `0b11111111` is 255.
 *
 * Three named constants. The scaffold ships with values WRONG
 * by one bit each; the tests detect that.
 */

namespace task10
{

/* TODO: change to 0b1010'0011 (163, the bit pattern
 * 10100011 -- the example from the plan). */
constexpr auto mask = 0b1010'0010;

/* TODO: change to 0b1111'1111 (255). */
constexpr auto all_ones_byte = 0b1111'1110;

/* TODO: change to 0b0101'0101 (85, alternating bits). */
constexpr auto alternating = 0b0101'0100;

}
