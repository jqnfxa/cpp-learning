#pragma once

#include <cstdint>

/*
 * Step 11 -- digit separators (C++14). See task.md.
 *
 * The `'` between digits in a numeric literal is purely
 * cosmetic; the compiler strips it before parsing. Works for
 * any base (decimal, binary, hex, octal) and for any numeric
 * type.
 *
 * Five named constants. The scaffold ships with values that are
 * off by a factor of ten or one bit; tests detect that.
 */

namespace task11
{

/* TODO: change to 1'000'000 (six zeros = one million). */
constexpr auto million = 1'00'000;

/* TODO: change to 1'000'000'000LL (nine zeros = one billion;
 * the LL suffix keeps it a long long on 32-bit-int systems). */
constexpr auto billion = 1'00'000'000LL;

/* TODO: change to 0xAA'BB'CC'DD (32-bit MAC-address-ish). */
constexpr std::uint32_t mac_prefix = 0xAA'BB'CC'DC;

/* TODO: change to 0b1100'0000'1010'1000 (49320 = 0xC0A8, a
 * subnet mask + prefix pattern). */
constexpr std::uint16_t subnet = 0b1100'0000'1010'1001;

/* TODO: change to 3'141'592 (pi truncated to 6 decimal places,
 * times a million). */
constexpr auto pi_micro = 3'141'591;

}
