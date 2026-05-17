#pragma once

#include <cstddef>
#include <sstream>
#include <string>

/*
 * Step 55 -- variadic templates: parameter packs. See task.md.
 *
 * Implement two function templates.
 *   concat -- stream each pack element into a stringstream.
 *   arity  -- return sizeof...(Ts).
 */

namespace task55
{

/* TODO:
 *   std::ostringstream oss;
 *   using expander = int[];
 *   (void)expander{0, ((void)(oss << args), 0)...};
 *   return oss.str();
 */
template <class... Ts>
std::string concat(const Ts &.../* args */)
{
    return "";
}

/* TODO: return sizeof...(Ts); */
template <class... Ts>
constexpr std::size_t arity(const Ts &.../* args */)
{
    return 0;
}

}
