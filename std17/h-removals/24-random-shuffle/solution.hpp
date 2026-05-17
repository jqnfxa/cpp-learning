#pragma once

#include <algorithm>
#include <random>
#include <vector>

/*
 * Step 24 -- replace random_shuffle with shuffle + engine.
 *
 * `std::random_shuffle` was removed in C++17. Use
 * `std::shuffle(first, last, engine)` with an explicit
 * random-bit engine.
 */

namespace task24
{

/* TODO:
 *   std::mt19937 rng(seed);
 *   std::shuffle(v.begin(), v.end(), rng);
 *   return v;
 */
/* TODO:
 *   std::mt19937 rng(seed);
 *   std::shuffle(v.begin(), v.end(), rng);
 *   return v;
 */
inline std::vector<int> shuffle_copy(std::vector<int> v, std::uint32_t seed)
{
    (void)seed;
    return v;
}

}
