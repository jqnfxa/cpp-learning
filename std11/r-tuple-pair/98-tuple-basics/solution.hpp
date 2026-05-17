#pragma once

#include <string>
#include <tuple>
#include <utility>

/*
 * Step 98 -- std::tuple basics. See task.md for the spec.
 *
 * Construct a record tuple from three arguments and return it.
 */

namespace task98
{

using record = std::tuple<int, std::string, double>;

/* TODO: return record(id, std::move(name), score); */
inline record make_record(int /*id*/, std::string /*name*/, double /*score*/)
{
    return record{};
}

}
