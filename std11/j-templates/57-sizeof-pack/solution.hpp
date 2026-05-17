#pragma once

#include <cstddef>

/*
 * Step 57 -- sizeof...(Pack). See task.md.
 *
 * Implement count_args, count_types, and type_list::size using
 * sizeof... on a parameter pack.
 */

namespace task57
{

/* TODO: return sizeof...(Ts); */
template <class... Ts>
constexpr std::size_t count_args(const Ts &.../* args */)
{
    return 0;
}

/* TODO: return sizeof...(Ts); */
template <class... Ts>
constexpr std::size_t count_types()
{
    return 0;
}

template <class... Ts>
struct type_list
{
    /* TODO: static constexpr std::size_t size = sizeof...(Ts); */
    static constexpr std::size_t size = 0;
};

}
