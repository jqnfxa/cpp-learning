#pragma once

#include <cctype>
#include <string_view>

/*
 * Step 33 -- std::string_view (C++17).
 *
 * Non-owning view of a contiguous char range. Cheaper than
 * `std::string&` for read-only parameters: no copy, accepts
 * std::string, char ptr, literal, and array uniformly.
 */

namespace task33
{

/* TODO: implement word-counting on a string_view.
 *   - A "word" is a maximal run of non-whitespace.
 *   - Whitespace per std::isspace.
 */
/* TODO: count maximal runs of non-whitespace.
 *   std::size_t count = 0; bool in_word = false;
 *   for (char c : s) {
 *     if (std::isspace((unsigned char)c)) in_word = false;
 *     else if (!in_word) { in_word = true; ++count; }
 *   }
 *   return count;
 */
inline std::size_t count_words(std::string_view s)
{
    (void)s;
    return 0;
}

}
