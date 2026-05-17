#pragma once

#include <type_traits>

/*
 * Step 21 -- _t alias forms of type traits (C++14). See task.md.
 *
 * C++14 added `_t` aliases for every trait that has a ::type
 * member. They turn the C++11 incantation
 *
 *   typename std::remove_cv<T>::type
 *
 * into
 *
 *   std::remove_cv_t<T>
 *
 * Two functions and a few internal aliases that use the C++14
 * form. The scaffold ships with stubs that use the C++11
 * `typename T::type` form (correct but verbose); the tests
 * primarily check VALUE correctness, so the stub passes them.
 * The pedagogical point is the code-level change.
 *
 * To make the stub-vs-reference distinction TESTABLE, the
 * stub also makes one trait wrong: `bare_type` removes the
 * pointer (wrong) instead of the cv-ref (right).
 */

namespace task21
{

/*
 * bare_type<T> -- strip references and top-level cv-qualifiers
 * from T. Equivalent to std::decay's behaviour minus
 * array-to-pointer and function-to-pointer decay.
 *
 * C++11 form: typename std::remove_cv<typename std::remove_reference<T>::type>::type
 * C++14 form: std::remove_cv_t<std::remove_reference_t<T>>
 */
template <class T>
/* TODO: replace with std::remove_cv_t<std::remove_reference_t<T>> */
using bare_type = typename std::remove_pointer<T>::type; /* stub: wrong trait */

/*
 * conditional_t<Cond, T, U> -- a C++14 alias for the
 * C++11 `typename std::conditional<Cond, T, U>::type`.
 *
 * Pick T if Cond, otherwise U.
 */
template <bool Cond, class T, class U>
/* TODO: replace with std::conditional_t<Cond, T, U> */
using pick = typename std::conditional<Cond, T, U>::type;

/*
 * enable_if_t<Cond, T> -- C++14 alias for the C++11
 * `typename std::enable_if<Cond, T>::type`.
 *
 * Used in SFINAE-based overloads. The C++14 form is shorter.
 */
template <bool Cond, class T = void>
/* TODO: replace with std::enable_if_t<Cond, T> */
using only_if = typename std::enable_if<Cond, T>::type;

/*
 * Returns true if T is "bare" in the sense of bare_type<T>:
 * stripping references and cv-qualifiers yields T itself.
 *
 * Easy way to spot "is this already a plain type" at compile
 * time.
 */
template <class T>
constexpr bool is_bare_v = std::is_same<T, bare_type<T>>::value;

}
