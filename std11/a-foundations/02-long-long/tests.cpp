#include <gtest/gtest.h>

#include <climits>

#include "solution.hpp"

/* Language-level guarantees about `long long`, pinned at compile time. */
static_assert(sizeof(long long) >= 8, "C++11 requires long long to be at least 64 bits wide");
static_assert(sizeof(long long) >= sizeof(long), "long long must be at least as wide as long");
static_assert(LLONG_MAX >= 9223372036854775807LL, "long long must hold at least 2^63 - 1");
static_assert(LLONG_MIN <= -9223372036854775807LL - 1, "long long must hold at least -2^63");

TEST(long_long, trivial_product)
{
    EXPECT_EQ(task02::mul64(2, 3), 6LL);
}

TEST(long_long, zero_product)
{
    EXPECT_EQ(task02::mul64(0, 100), 0LL);
    EXPECT_EQ(task02::mul64(100, 0), 0LL);
}

TEST(long_long, negative_product)
{
    EXPECT_EQ(task02::mul64(-3, 7), -21LL);
    EXPECT_EQ(task02::mul64(-4, -5), 20LL);
}

TEST(long_long, overflows_int)
{
    /* 100000 * 100000 = 10^10, comfortably above INT_MAX. */
    EXPECT_EQ(task02::mul64(100000, 100000), 10000000000LL);
}

TEST(long_long, int_max_doubled)
{
    EXPECT_EQ(task02::mul64(INT_MAX, 2), 4294967294LL);
}

TEST(long_long, int_min_negated)
{
    /*
     * INT_MIN is -2^31; the correct result is +2^31, which is one past
     * INT_MAX. A solution that computes a * b in int first hits signed
     * overflow (UB) here -- the cast must come before the multiply.
     */
    EXPECT_EQ(task02::mul64(INT_MIN, -1), 2147483648LL);
}
