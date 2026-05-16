#include <gtest/gtest.h>

#include "solution.hpp"

/*
 * Bring the operators into scope. UDLs are namespaced like any other
 * function, so a using directive (or explicit `using ::operator"" _km;`)
 * is needed in the calling code.
 */
using namespace task41;

static_assert(1_km == 1000LL, "1_km == 1000");
static_assert(0_km == 0LL, "0_km == 0");
static_assert(42_km == 42000LL, "42_km == 42000");
static_assert(1_h == 3600LL, "1_h == 3600");
static_assert(24_h == 86400LL, "24_h == 86400");

/* Arithmetic combinations remain constant expressions. */
static_assert(2_km + 500 == 2500LL, "2_km + 500 == 2500");
static_assert(1_h * 7 == 25200LL, "1_h * 7 == 25200 (a week)");

TEST(udl_integer, km_basic)
{
    EXPECT_EQ(1_km, 1000LL);
    EXPECT_EQ(42_km, 42000LL);
    EXPECT_EQ(0_km, 0LL);
}

TEST(udl_integer, h_basic)
{
    EXPECT_EQ(1_h, 3600LL);
    EXPECT_EQ(24_h, 86400LL);
    EXPECT_EQ(0_h, 0LL);
}

TEST(udl_integer, arithmetic_with_literals)
{
    EXPECT_EQ(2_km + 500, 2500LL);
    EXPECT_EQ(5_km - 1_km, 4000LL);
    EXPECT_EQ(3_h + 30 * 60, 12600LL); /* 3 hours + 30 minutes = 12600 seconds */
}

TEST(udl_integer, return_type_is_long_long)
{
    /* If the implementation returned `int`, large multiplications
     * could overflow. long long must be the actual return type. */
    auto x = 1000000_km; /* 1e9 meters */
    EXPECT_EQ(x, 1000000000LL);
    static_assert(std::is_same<decltype(1_km), long long>::value, "1_km has type long long");
}
