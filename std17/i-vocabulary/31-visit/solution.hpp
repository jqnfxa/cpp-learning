#pragma once

#include <string>
#include <variant>

/*
 * Step 31 -- std::visit (C++17).
 *
 * Apply a callable to whichever alternative is held. The
 * callable must handle ALL alternatives (often via if
 * constexpr or an overload-set helper).
 */

namespace task31
{

using value = std::variant<int, std::string>;

/*
 * Reference:
 *   return std::visit([](auto const& v) -> std::string {
 *     if constexpr (std::is_same_v<std::decay_t<decltype(v)>, int>)
 *       return "int:" + std::to_string(v);
 *     else
 *       return "str:" + v;
 *   }, val);
 */
inline std::string render(value const &val)
{
    /* TODO: visit with a generic lambda + if constexpr. See
     * the top-of-file comment. */
    (void)val;
    return "";
}

}
