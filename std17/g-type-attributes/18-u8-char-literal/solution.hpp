#pragma once

/*
 * Step 18 -- u8 character literals (C++17). See task.md.
 *
 * `u8'A'` is a char literal with UTF-8 encoding. In C++17,
 * its type is `char` (in C++20 it became `char8_t`).
 *
 * The value is the UTF-8 code unit for the character. For
 * ASCII characters, it's the ASCII value.
 */

namespace task18
{

/* TODO: return u8'A'. */
constexpr char letter_a()
{
    return '\0';
}

/* TODO: return u8'0'. */
constexpr char digit_zero()
{
    return '\0';
}

/* TODO: return u8'\n' (newline). */
constexpr char newline()
{
    return '\0';
}

}
