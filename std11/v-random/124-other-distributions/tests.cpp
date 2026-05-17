#include <gtest/gtest.h>

#include "solution.hpp"

TEST(other_distributions, normal_zero_one_mean_near_zero)
{
    /* 1000 draws from N(0, 1). SE = 1/sqrt(1000) ~ 0.032.
     * 3-sigma band: [-0.10, 0.10]. We use 0.15 to be safe. */
    double m = task124::normal_mean(42, 1000, 0.0, 1.0);
    EXPECT_GT(m, -0.15);
    EXPECT_LT(m, 0.15);
}

TEST(other_distributions, normal_shifted_mean)
{
    /* 1000 draws from N(5, 2). Sample mean near 5. SE = 2/sqrt(1000)
     * ~ 0.063. Band [5 - 0.2, 5 + 0.2]. */
    double m = task124::normal_mean(42, 1000, 5.0, 2.0);
    EXPECT_GT(m, 4.8);
    EXPECT_LT(m, 5.2);
}

TEST(other_distributions, bernoulli_half_near_half)
{
    /* 1000 draws from Bern(0.5). SE = sqrt(0.25/1000) ~ 0.016.
     * Band [0.45, 0.55] is comfortable. */
    double p = task124::bernoulli_mean(42, 1000, 0.5);
    EXPECT_GT(p, 0.45);
    EXPECT_LT(p, 0.55);
}

TEST(other_distributions, bernoulli_low_p)
{
    /* p = 0.1: SE = sqrt(0.09/1000) ~ 0.0095. Band [0.07, 0.13]. */
    double p = task124::bernoulli_mean(42, 1000, 0.1);
    EXPECT_GT(p, 0.07);
    EXPECT_LT(p, 0.13);
}

TEST(other_distributions, bernoulli_high_p)
{
    double p = task124::bernoulli_mean(42, 1000, 0.9);
    EXPECT_GT(p, 0.87);
    EXPECT_LT(p, 0.93);
}

TEST(other_distributions, poisson_mean_near_lambda)
{
    /* Poisson(5): mean and variance both 5. SE = sqrt(5/1000) ~ 0.071.
     * Band [5 - 0.25, 5 + 0.25]. */
    double m = task124::poisson_mean(42, 1000, 5.0);
    EXPECT_GT(m, 4.75);
    EXPECT_LT(m, 5.25);
}

TEST(other_distributions, poisson_large_lambda)
{
    /* Poisson(50): SE = sqrt(50/1000) ~ 0.22. Band [49.3, 50.7]. */
    double m = task124::poisson_mean(42, 1000, 50.0);
    EXPECT_GT(m, 49.3);
    EXPECT_LT(m, 50.7);
}

TEST(other_distributions, same_seed_gives_same_results)
{
    /* Each function with the same seed must produce identical
     * sample means. */
    EXPECT_DOUBLE_EQ(task124::normal_mean(42, 1000, 0.0, 1.0),
                     task124::normal_mean(42, 1000, 0.0, 1.0));
    EXPECT_DOUBLE_EQ(task124::bernoulli_mean(42, 1000, 0.5),
                     task124::bernoulli_mean(42, 1000, 0.5));
    EXPECT_DOUBLE_EQ(task124::poisson_mean(42, 1000, 5.0),
                     task124::poisson_mean(42, 1000, 5.0));
}
