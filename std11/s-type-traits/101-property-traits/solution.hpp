#pragma once

#include <string>
#include <type_traits>

/*
 * Step 101 -- property type traits. See task.md for the spec.
 *
 * Classify T as "integral", "floating_point", "pointer", or "other".
 */

namespace task101
{

/* TODO:
 *   if (std::is_integral<T>::value)       return "integral";
 *   if (std::is_floating_point<T>::value) return "floating_point";
 *   if (std::is_pointer<T>::value)        return "pointer";
 *   return "other";
 */
template <class T>
std::string name_kind()
{
    return "other";
}

}
