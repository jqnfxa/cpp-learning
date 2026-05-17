#pragma once

#include <string>
#include <type_traits>

/*
 * Step 103 -- std::enable_if + SFINAE. See task.md for the spec.
 *
 * Two overloads of to_int: one template guarded by
 * std::enable_if<std::is_arithmetic<T>::value>, one non-template
 * for std::string.
 */

namespace task103
{

/* TODO:
 *   typename std::enable_if<std::is_arithmetic<T>::value, int>::type
 *   to_int(T x) { return static_cast<int>(x); }
 */
template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, int>::type to_int(T /*x*/)
{
    return 0;
}

/* TODO: return std::stoi(s); */
inline int to_int(const std::string & /*s*/)
{
    return 0;
}

}
