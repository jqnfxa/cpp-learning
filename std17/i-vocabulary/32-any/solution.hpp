#pragma once

#include <any>
#include <string>

/*
 * Step 32 -- std::any (C++17).
 *
 * Holds a value of ANY type. Type-erased; access via
 * std::any_cast (throws bad_any_cast on type mismatch).
 */

namespace task32
{

/* TODO: implement as `return std::any{x};` etc.
 *   any_cast<int>(a) extracts. */
inline std::any wrap_int(int x)
{
    (void)x;
    return {};
}
inline std::any wrap_string(std::string s)
{
    (void)s;
    return {};
}
inline int unwrap_int(const std::any &a)
{
    (void)a;
    return 0;
}

}
