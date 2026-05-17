#pragma once

#include <string>
#include <tuple>

/*
 * Step 99 -- std::get<I> and std::tie. See task.md for the spec.
 *
 * Implement three positional getters and a tie-based unpacker.
 */

namespace task99
{

using record = std::tuple<int, std::string, double>;

/* TODO: return std::get<0>(r); */
inline int first_field(const record & /*r*/)
{
    return 0;
}

/* TODO: return std::get<1>(r); */
inline std::string second_field(const record & /*r*/)
{
    return {};
}

/* TODO: return std::get<2>(r); */
inline double third_field(const record & /*r*/)
{
    return 0.0;
}

/* TODO: std::tie(id, name, score) = r; */
inline void unpack(const record & /*r*/, int & /*id*/, std::string & /*name*/, double & /*score*/)
{
}

}
