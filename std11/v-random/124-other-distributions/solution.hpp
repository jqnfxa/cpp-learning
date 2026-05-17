#pragma once

#include <cstdint>
#include <random>

/*
 * Step 124 -- normal / bernoulli / poisson distributions.
 *
 * Each function draws n samples from the named distribution with the
 * given parameters and returns the sample mean (or the success
 * proportion for Bernoulli).
 */

namespace task124
{

/* TODO:
 *   std::mt19937 rng(seed);
 *   std::normal_distribution<double> dist(mean, stddev);
 *   double sum = 0;
 *   for (int i = 0; i < n; ++i) sum += dist(rng);
 *   return sum / n;
 */
inline double normal_mean(std::uint_fast32_t /*seed*/, int /*n*/, double /*mean*/,
                          double /*stddev*/)
{
    return 0.0;
}

/* TODO: same shape with std::bernoulli_distribution(p) and return
 *       the proportion of true results. */
inline double bernoulli_mean(std::uint_fast32_t /*seed*/, int /*n*/, double /*p*/)
{
    return 0.0;
}

/* TODO: same shape with std::poisson_distribution<int>(lambda). */
inline double poisson_mean(std::uint_fast32_t /*seed*/, int /*n*/, double /*lambda*/)
{
    return 0.0;
}

}
