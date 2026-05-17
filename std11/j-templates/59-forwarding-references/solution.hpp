#pragma once

#include <type_traits>

/*
 * Step 59 -- forwarding references. See task.md.
 *
 * Both helpers use a forwarding-reference parameter (T &&) and
 * inspect T to determine the category of the original argument.
 */

namespace task59
{

/* TODO: return is_lvalue_reference<T>::value ? "lvalue" : "rvalue"; */
template <class T>
const char *category(T && /* arg */)
{
    return "?";
}

/* TODO: return is_reference<T>::value ? "T_amp" : "T"; */
template <class T>
const char *deduced_kind(T && /* arg */)
{
    return "?";
}

}
