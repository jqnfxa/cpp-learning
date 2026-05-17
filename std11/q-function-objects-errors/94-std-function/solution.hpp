#pragma once

#include <functional>
#include <string>
#include <unordered_map>

/*
 * Step 94 -- std::function. See task.md for the spec.
 *
 * Implement a name -> int(int) dispatch table and an apply function.
 */

namespace task94
{

using op = std::function<int(int)>;
using dispatch_table = std::unordered_map<std::string, op>;

/* TODO: return a brace-init table with five entries:
 *   "negate" -> -x, "double" -> 2x, "square" -> x*x,
 *   "succ"   -> x+1, "pred" -> x-1. */
inline dispatch_table make_arithmetic_table()
{
    return {};
}

/* TODO: return t.at(name)(x);  (at() throws std::out_of_range on miss). */
inline int apply(const dispatch_table & /*t*/, const std::string & /*name*/, int /*x*/)
{
    return 0;
}

}
