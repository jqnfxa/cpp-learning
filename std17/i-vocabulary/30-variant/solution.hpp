#pragma once

#include <cstddef>
#include <string>
#include <variant>

/*
 * Step 30 -- std::variant<Ts...> (C++17).
 *
 * A tagged union. Each variant instance holds exactly one of
 * the alternatives. `.index()` returns which.
 */

namespace task30
{

using json_value = std::variant<std::nullptr_t, bool, double, std::string>;

/* TODO: implement each factory to return the appropriate
 * alternative of the variant. */
inline json_value make_null()
{
    return false;
}
inline json_value make_bool(bool b)
{
    (void)b;
    return nullptr;
}
inline json_value make_number(double d)
{
    (void)d;
    return nullptr;
}
inline json_value make_string(std::string s)
{
    (void)s;
    return nullptr;
}

}
