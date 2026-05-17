#include <gtest/gtest.h>

#include <cstdint>
#include <set>
#include <vector>

#include "solution.hpp"

TEST(random_engines, generate_8_returns_eight_values)
{
    auto v = task122::generate_8();
    EXPECT_EQ(v.size(), 8u);
}

TEST(random_engines, generate_8_two_calls_typically_differ)
{
    /* Two fresh seedings from random_device almost certainly produce
     * different sequences. Astronomically unlikely to collide. */
    auto a = task122::generate_8();
    auto b = task122::generate_8();
    EXPECT_NE(a, b)
        << "two random_device-seeded sequences must (almost surely) differ";
}

TEST(random_engines, generate_8_seeded_is_deterministic)
{
    /* Same seed -> same sequence, every call, within one binary. */
    auto a = task122::generate_8_seeded(42);
    auto b = task122::generate_8_seeded(42);
    EXPECT_EQ(a, b);
}

TEST(random_engines, generate_8_seeded_returns_eight_values)
{
    auto v = task122::generate_8_seeded(123);
    EXPECT_EQ(v.size(), 8u);
}

TEST(random_engines, different_seeds_give_different_sequences)
{
    auto a = task122::generate_8_seeded(1);
    auto b = task122::generate_8_seeded(2);
    EXPECT_NE(a, b);
}

TEST(random_engines, seeded_values_are_not_all_zero)
{
    /* A correctly-seeded mt19937 produces full-range outputs. The
     * default-constructed sentinel of {} would be all-zero -- the
     * stub returns that. Reference returns large non-zero values. */
    auto v = task122::generate_8_seeded(42);
    ASSERT_EQ(v.size(), 8u);
    bool all_zero = true;
    for (auto x : v)
    {
        if (x != 0)
        {
            all_zero = false;
            break;
        }
    }
    EXPECT_FALSE(all_zero) << "engine output should not be all-zero";
}

TEST(random_engines, seeded_values_are_distinct)
{
    /* 8 outputs from mt19937 with seed 42 are independent random
     * uint32s. The probability of any collision among 8 32-bit
     * values is < 8 * 2^-32 ~ 1.86e-9. Treat as deterministic. */
    auto v = task122::generate_8_seeded(42);
    std::set<std::uint_fast32_t> uniq(v.begin(), v.end());
    EXPECT_EQ(uniq.size(), 8u);
}
