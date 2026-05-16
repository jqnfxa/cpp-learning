#include <gtest/gtest.h>

#include <climits>

#include "solution.hpp"

/*
 * Compile-time block. If is_power_of_two is wrong, these fire BEFORE
 * any TEST runs and you see the error in the build log, not in the
 * test output.
 */

/* powers of two: should be true */
static_assert(task05::is_power_of_two(1), "1 is 2^0");
static_assert(task05::is_power_of_two(2), "2 is 2^1");
static_assert(task05::is_power_of_two(4), "4 is 2^2");
static_assert(task05::is_power_of_two(8), "8 is 2^3");
static_assert(task05::is_power_of_two(16), "16 is 2^4");
static_assert(task05::is_power_of_two(1024), "1024 is 2^10");
static_assert(task05::is_power_of_two(1ULL << 32), "2^32");
static_assert(task05::is_power_of_two(1ULL << 63), "2^63 -- highest bit set, still a power of two");

/* non-powers of two: should be false */
static_assert(!task05::is_power_of_two(0), "0 is not a power of two by convention");
static_assert(!task05::is_power_of_two(3), "3 = 2 + 1 (two bits set)");
static_assert(!task05::is_power_of_two(5), "5 = 4 + 1");
static_assert(!task05::is_power_of_two(6), "6 = 4 + 2");
static_assert(!task05::is_power_of_two(7), "7 = 4 + 2 + 1");
static_assert(!task05::is_power_of_two(15), "15 = 0b1111");
static_assert(!task05::is_power_of_two(1023), "one shy of 1024");
static_assert(!task05::is_power_of_two(1025), "one over 1024");
static_assert(!task05::is_power_of_two(ULLONG_MAX), "all bits set");

TEST(static_assert_predicate, runtime_every_power_of_two_up_to_63)
{
    for (int p = 0; p < 64; ++p)
    {
        EXPECT_TRUE(task05::is_power_of_two(1ULL << p)) << "p = " << p;
    }
}

TEST(static_assert_predicate, runtime_zero_is_not_a_power)
{
    EXPECT_FALSE(task05::is_power_of_two(0));
}

TEST(static_assert_predicate, runtime_small_non_powers)
{
    EXPECT_FALSE(task05::is_power_of_two(3));
    EXPECT_FALSE(task05::is_power_of_two(5));
    EXPECT_FALSE(task05::is_power_of_two(6));
    EXPECT_FALSE(task05::is_power_of_two(7));
}

TEST(static_assert_predicate, runtime_around_a_power_of_two)
{
    EXPECT_FALSE(task05::is_power_of_two(1023));
    EXPECT_FALSE(task05::is_power_of_two(1025));
    EXPECT_FALSE(task05::is_power_of_two(100));
}

TEST(static_assert_predicate, runtime_max_unsigned)
{
    EXPECT_FALSE(task05::is_power_of_two(ULLONG_MAX));
}
