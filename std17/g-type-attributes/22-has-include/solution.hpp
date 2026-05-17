#pragma once

/*
 * Step 22 -- __has_include (C++17). See task.md.
 *
 * The preprocessor expression `__has_include(<header>)`
 * evaluates to 1 if <header> can be found, 0 otherwise.
 * Lets headers adapt to the available standard library.
 */

/* TODO: replace the unconditional `#include <optional>` with
 * `#if __has_include(<optional>)` ... `#endif` and provide a
 * fallback for the negative branch.
 *
 * Reference:
 *
 *   #if __has_include(<optional>)
 *   #  include <optional>
 *   #  define TASK22_HAS_OPTIONAL 1
 *   #else
 *   #  define TASK22_HAS_OPTIONAL 0
 *   #endif
 */
#include <optional>

namespace task22
{

/* Reports whether the host stdlib has <optional>. */
constexpr bool has_optional()
{
#if __has_include(<optional>)
    return true;
#else
    return false;
#endif
}

/* Reports whether <a_header_that_does_not_exist> is
 * available. Should always be false. */
constexpr bool has_nonexistent_header()
{
#if __has_include(<this_header_does_not_exist_anywhere.hpp>)
    return true;
#else
    return false;
#endif
}

/* TODO: return std::optional<int>{value}; */
/* TODO: return std::optional<int>{value}. */
inline std::optional<int> make_optional_int(int value)
{
    (void)value;
    return std::nullopt;
}

}
