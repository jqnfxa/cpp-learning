#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

/*
 * Step 14 -- std::integer_sequence / std::index_sequence
 * (C++14). See task.md.
 *
 * `std::index_sequence<I1, I2, ...>` is a compile-time list of
 * size_t values. `std::make_index_sequence<N>` builds
 * `index_sequence<0, 1, ..., N-1>`. `std::index_sequence_for<Args...>`
 * builds the sequence matching a parameter pack.
 *
 * Two demo functions, both using index_sequence to unpack a tuple:
 *   - apply_sum:     summation of all elements
 *   - apply_product: product of all elements
 *
 * The scaffold ships with stubs returning the identity (0 for
 * sum, 1 for product) regardless of the tuple contents.
 * Tests detect that for any non-trivial tuple.
 */

namespace task14
{

namespace detail
{

template <typename Tuple, std::size_t... Is>
int sum_impl(Tuple const &t, std::index_sequence<Is...>)
{
    /* TODO: sum all std::get<Is>(t) using the dummy-array trick
     * (C++14 lacks fold expressions; C++17 would just write
     * `(std::get<Is>(t) + ...)`).
     *
     *   int result = 0;
     *   int dummy[] = {0, (result += std::get<Is>(t), 0)...};
     *   (void)dummy;
     *   return result;
     */
    (void)t;
    return 0;
}

template <typename Tuple, std::size_t... Is>
int product_impl(Tuple const &t, std::index_sequence<Is...>)
{
    /* TODO: product of all std::get<Is>(t).
     *
     *   int result = 1;
     *   int dummy[] = {0, (result *= std::get<Is>(t), 0)...};
     *   (void)dummy;
     *   return result;
     */
    (void)t;
    return 1;
}

}

/*
 * Sum every element of `t` using std::index_sequence_for to
 * build the compile-time index pack matching the tuple's
 * arity.
 */
template <typename... Ts>
int apply_sum(std::tuple<Ts...> const &t)
{
    return detail::sum_impl(t, std::index_sequence_for<Ts...>{});
}

/*
 * Same idea with multiplication.
 */
template <typename... Ts>
int apply_product(std::tuple<Ts...> const &t)
{
    return detail::product_impl(t, std::index_sequence_for<Ts...>{});
}

/*
 * Sum the first N elements of a tuple (a common pattern when
 * a tuple holds heterogeneous data and you only want a prefix).
 * Uses `make_index_sequence<N>` directly rather than
 * `index_sequence_for`.
 */
template <std::size_t N, typename Tuple>
int sum_first_n(Tuple const &t)
{
    return detail::sum_impl(t, std::make_index_sequence<N>{});
}

}
