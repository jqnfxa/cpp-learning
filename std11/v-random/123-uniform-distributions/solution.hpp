#pragma once

#include <array>
#include <cstdint>
#include <random>

/*
 * Step 123 -- uniform_int / uniform_real distributions. See task.md.
 *
 * roll_dice_histogram: 1..6 dice rolls into a size-7 histogram.
 * uniform_real_sample: one draw from [lo, hi).
 */

namespace task123
{

/* TODO:
 *   std::mt19937 rng(seed);
 *   std::uniform_int_distribution<int> dice(1, 6);
 *   std::array<int, 7> hist{};
 *   for (int i = 0; i < rolls; ++i) ++hist[dice(rng)];
 *   return hist;
 */
inline std::array<int, 7> roll_dice_histogram(std::uint_fast32_t /*seed*/, int /*rolls*/)
{
    return std::array<int, 7>{};
}

/* TODO:
 *   std::mt19937 rng(seed);
 *   std::uniform_real_distribution<double> dist(lo, hi);
 *   return dist(rng);
 */
inline double uniform_real_sample(std::uint_fast32_t /*seed*/, double /*lo*/, double /*hi*/)
{
    return 0.0;
}

}
