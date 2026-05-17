#pragma once

#include <cstdint>
#include <random>
#include <vector>

/*
 * Step 122 -- mt19937 engine + random_device seeding. See task.md.
 *
 * Two functions: one seeded from std::random_device (entropy), one
 * from a caller-provided integer (deterministic).
 */

namespace task122
{

/* TODO:
 *   std::random_device dev;
 *   std::mt19937 rng(dev());
 *   std::vector<std::uint_fast32_t> out;
 *   out.reserve(8);
 *   for (int i = 0; i < 8; ++i) out.push_back(rng());
 *   return out;
 */
inline std::vector<std::uint_fast32_t> generate_8()
{
    return {};
}

/* TODO: same shape but seed mt19937 with `seed` directly. */
inline std::vector<std::uint_fast32_t> generate_8_seeded(std::uint_fast32_t /*seed*/)
{
    return {};
}

}
