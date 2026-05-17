#pragma once

#include <string>
#include <tuple>

/*
 * Step 23 -- std::get<T>(tuple) (C++14). See task.md.
 *
 * C++11 had std::get<I>(tuple) (by index). C++14 added
 * std::get<T>(tuple) (by type). The by-type form is
 * unambiguous only when T occurs exactly once in the tuple's
 * element types.
 *
 * The scaffold ships with stubs that use the by-index form
 * with the WRONG INDEX -- get<0> for the string, get<2> for
 * the int, etc. Tests catch the mismatch.
 */

namespace task23
{

using profile = std::tuple<int, std::string, double>;

inline profile make_profile()
{
    return std::make_tuple(42, std::string("alice"), 3.14);
}

/*
 * Get the int field of a profile by TYPE.
 *
 * C++11 form: std::get<0>(p) (by index, fragile to tuple
 * reordering).
 * C++14 form: std::get<int>(p) (by type, robust).
 */
inline int get_age(const profile & /*p*/)
{
    /* TODO: return std::get<int>(p); */
    return 0; /* stub: hardcoded wrong value */
}

inline std::string get_name(const profile & /*p*/)
{
    /* TODO: return std::get<std::string>(p); */
    return ""; /* stub */
}

inline double get_score(const profile & /*p*/)
{
    /* TODO: return std::get<double>(p); */
    return 0.0; /* stub */
}

}
