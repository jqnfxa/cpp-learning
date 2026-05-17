#pragma once

#include <algorithm>
#include <initializer_list>

/*
 * Step 56 -- pack expansion. See task.md.
 *
 * Implement min_of / max_of using pack expansion into a brace-init
 * list passed to std::min / std::max.
 */

namespace task56
{

/* TODO: return std::min<T>({first, rest...}); */
template <class T, class... Rest>
T min_of(T first, Rest... /* rest */)
{
    return first;
}

/* TODO: return std::max<T>({first, rest...}); */
template <class T, class... Rest>
T max_of(T first, Rest... /* rest */)
{
    return first;
}

}
