#pragma once

#include <type_traits>

/*
 * Step 102 -- transformation traits. See task.md for the spec.
 *
 * Define bare_type<T>::type as remove_cv<remove_reference<T>::type>::type.
 */

namespace task102
{

/* TODO:
 *   using type = typename std::remove_cv<
 *       typename std::remove_reference<T>::type
 *   >::type;
 */
template <class T>
struct bare_type
{
    using type = T;
};

}
