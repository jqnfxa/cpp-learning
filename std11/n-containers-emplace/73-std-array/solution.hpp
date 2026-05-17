#pragma once

#include <array>

/*
 * Step 73 -- std::array<T, N>. See task.md for the spec.
 *
 * Implement sum / mean / component-wise add for std::array<double, 4>.
 */

namespace task73
{

/* TODO: return the sum of the four elements. */
inline double sum(const std::array<double, 4> & /*a*/)
{
    return 0.0;
}

/* TODO: return sum(a) / 4.0. */
inline double mean(const std::array<double, 4> & /*a*/)
{
    return 0.0;
}

/* TODO: return {x[0]+y[0], x[1]+y[1], x[2]+y[2], x[3]+y[3]}. */
inline std::array<double, 4> add(const std::array<double, 4> & /*x*/,
                                 const std::array<double, 4> & /*y*/)
{
    return std::array<double, 4>{};
}

}
