#include <gtest/gtest.h>

#include <array>
#include <numeric>

#include "solution.hpp"

TEST(uniform_distributions, dice_total_equals_rolls)
{
    auto hist = task123::roll_dice_histogram(42, 1000);
    int total = std::accumulate(hist.begin(), hist.end(), 0);
    EXPECT_EQ(total, 1000);
}

TEST(uniform_distributions, dice_zero_rolls)
{
    auto hist = task123::roll_dice_histogram(42, 0);
    for (int v : hist)
    {
        EXPECT_EQ(v, 0);
    }
}

TEST(uniform_distributions, dice_index_zero_unused)
{
    auto hist = task123::roll_dice_histogram(42, 1000);
    EXPECT_EQ(hist[0], 0) << "histogram index 0 must remain zero";
}

TEST(uniform_distributions, dice_buckets_one_to_six_all_hit)
{
    /* With 1000 rolls of a fair die, every bucket should be hit
     * many times. */
    auto hist = task123::roll_dice_histogram(42, 1000);
    for (int face = 1; face <= 6; ++face)
    {
        EXPECT_GT(hist[face], 0) << "face " << face << " never rolled";
    }
}

TEST(uniform_distributions, dice_buckets_in_plausible_range)
{
    /* 1000 rolls of a fair die -> each bucket near 167 expected.
     * 3-sigma binomial(1000, 1/6) is about 35; we use a loose
     * 120..220 band to avoid flaky failures on weird streaks. */
    auto hist = task123::roll_dice_histogram(42, 1000);
    for (int face = 1; face <= 6; ++face)
    {
        EXPECT_GE(hist[face], 120) << "face " << face;
        EXPECT_LE(hist[face], 220) << "face " << face;
    }
}

TEST(uniform_distributions, dice_same_seed_same_result)
{
    auto a = task123::roll_dice_histogram(42, 1000);
    auto b = task123::roll_dice_histogram(42, 1000);
    EXPECT_EQ(a, b);
}

TEST(uniform_distributions, dice_different_seeds_different_result)
{
    auto a = task123::roll_dice_histogram(1, 1000);
    auto b = task123::roll_dice_histogram(2, 1000);
    EXPECT_NE(a, b);
}

TEST(uniform_distributions, uniform_real_in_range)
{
    double v = task123::uniform_real_sample(42, 0.0, 1.0);
    EXPECT_GE(v, 0.0);
    EXPECT_LT(v, 1.0);
}

TEST(uniform_distributions, uniform_real_scaled_range)
{
    double v = task123::uniform_real_sample(42, -10.0, 10.0);
    EXPECT_GE(v, -10.0);
    EXPECT_LT(v, 10.0);
}

TEST(uniform_distributions, uniform_real_same_seed_same_value)
{
    EXPECT_DOUBLE_EQ(task123::uniform_real_sample(42, 0.0, 1.0),
                     task123::uniform_real_sample(42, 0.0, 1.0));
}
