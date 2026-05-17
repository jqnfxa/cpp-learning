#pragma once

#include <optional>
#include <string>

/*
 * Step 28 -- std::optional<T> (C++17).
 *
 * Returns either a value or "nothing" (std::nullopt) without
 * the verbosity of out-parameter + bool-return, or the
 * cost/danger of exceptions for predictable failures.
 */

namespace task28
{

/* TODO:
 *   try {
 *     return std::stoi(s);
 *   } catch (...) {
 *     return std::nullopt;
 *   }
 */
/* TODO:
 *   try {
 *     size_t pos = 0;
 *     int v = std::stoi(s, &pos);
 *     if (pos != s.size()) return std::nullopt;
 *     return v;
 *   } catch (...) {
 *     return std::nullopt;
 *   }
 */
inline std::optional<int> parse_int(const std::string &s)
{
    (void)s;
    return std::nullopt;
}

}
