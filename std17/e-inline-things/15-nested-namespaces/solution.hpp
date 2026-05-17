#pragma once

#include <string>

/*
 * Step 15 -- nested namespaces (C++17). See task.md.
 *
 * C++17 lets you collapse `namespace a { namespace b {
 * namespace c { ... }}}` into `namespace a::b::c { ... }`.
 *
 * Same semantics; less indentation.
 *
 * The scaffold returns wrong values; tests catch.
 */

namespace task15::deep::nest
{

/* TODO: return 42. */
inline int answer()
{
    return 0;
}

/* TODO: return "deep-nest". */
inline std::string label()
{
    return "";
}

}
