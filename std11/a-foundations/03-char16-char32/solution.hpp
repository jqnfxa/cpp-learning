#pragma once

#include <cstddef>

/*
 * Step 03 -- char16_t and char32_t. See task.md for the spec.
 *
 * Implement two strlen-equivalent functions for the C++11 wide-char
 * types. Both count *code units* (not code points), so the
 * supplementary-plane case in tests.cpp returns 2 for UTF-16 and 1
 * for UTF-32. Do not change the function signatures.
 */

namespace task03
{

inline std::size_t count_utf16_units(const char16_t * /* s */)
{
    /* TODO: walk the null-terminated string and count code units */
    return 0;
}

inline std::size_t count_utf32_units(const char32_t * /* s */)
{
    /* TODO: walk the null-terminated string and count code units */
    return 0;
}

}
