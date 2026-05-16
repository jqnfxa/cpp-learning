#pragma once

#include <type_traits>
#include <utility>

/*
 * Step 19 -- narrowing prevention with {}. See task.md.
 *
 * 1. Implement truncate_to_int using direct initialization (parens).
 * 2. The SFINAE trait below is provided -- tests.cpp uses it to pin
 *    the narrowing rules with static_assert.
 */

namespace task19
{

/*
 * is_brace_constructible_from<To, From> -- true iff `To{from}` is a
 * well-formed expression where `from` is a (notional) value of type
 * From. Narrowing conversions make the expression ill-formed, so
 * this trait reports `false` for narrowing pairs and `true` for
 * identity / widening / explicit-conversion pairs.
 */

template <class...>
using void_t = void;

template <class To, class From, class = void>
struct is_brace_constructible_from : std::false_type
{
};

template <class To, class From>
struct is_brace_constructible_from<To, From, void_t<decltype(To{std::declval<From>()})>> : std::true_type
{
};

/*
 * TODO: implement using direct initialization with parens.
 *   return int(d);   -- the paren form
 * The point: parens silently truncate. The brace form `int{d}` would
 * reject this at compile time.
 */
inline int truncate_to_int(double /* d */)
{
    return 0;
}

}
