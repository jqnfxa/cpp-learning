#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>

/*
 * Step 67 -- std::unique_ptr<T>. See task.md for the spec.
 *
 * Implement three functions using std::unique_ptr<int[]> so the
 * resulting array is owned, zero-initialised, summable, and
 * leak-free on early return / exception.
 */

namespace task67
{

/* TODO: return std::unique_ptr<int[]>(new int[n]()). */
inline std::unique_ptr<int[]> make_zeros(std::size_t /*n*/)
{
    return std::unique_ptr<int[]>();
}

/* TODO: sum the first n elements of arr. */
inline int sum(const std::unique_ptr<int[]> & /*arr*/, std::size_t /*n*/)
{
    return 0;
}

/* TODO: allocate locally, validate denominator, throw if zero,
 *       otherwise return sum/denominator. */
inline int safe_average(std::size_t /*n*/, int /*denominator*/)
{
    return 0;
}

}
