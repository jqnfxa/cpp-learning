#pragma once

#include <string>
#include <utility>
#include <vector>

/*
 * Step 78 -- move into a container. See task.md for the spec.
 *
 * Implement push_copy and push_move so tests can observe the
 * difference between v.push_back(s) and v.push_back(std::move(s)).
 */

namespace task78
{

/* TODO: v.push_back(s);  -- s is unchanged after the call. */
inline void push_copy(std::vector<std::string> & /*v*/, std::string & /*s*/)
{
}

/* TODO: v.push_back(std::move(s));  -- s is left empty / moved-from. */
inline void push_move(std::vector<std::string> & /*v*/, std::string & /*s*/)
{
}

}
