#pragma once

#include <iterator>
#include <string>

/*
 * Step 92 -- free std::begin / std::end. See task.md for the spec.
 *
 * Write to_string<C>(c) that formats the elements of c as
 * "[v0, v1, ...]", using free begin/end so it works for both
 * containers and C arrays.
 */

namespace task92
{

/* TODO:
 *   using std::begin;
 *   using std::end;
 *   for (auto it = begin(c); it != end(c); ++it) { ... std::to_string(*it) ... }
 */
template <class C>
std::string to_string(const C & /*c*/)
{
    return {};
}

}
