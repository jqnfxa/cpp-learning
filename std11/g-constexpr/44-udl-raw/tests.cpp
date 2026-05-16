#include <gtest/gtest.h>

#include "solution.hpp"

using namespace task44;

/* Compile-time checks. */
static_assert(0_bin == 0ULL, "0_bin == 0");
static_assert(1_bin == 1ULL, "1_bin == 1");
static_assert(10_bin == 2ULL, "10_bin == 2");
static_assert(11_bin == 3ULL, "11_bin == 3");
static_assert(1010_bin == 10ULL, "1010_bin == 10");
static_assert(11111111_bin == 255ULL, "8 ones == 255");

/* Digit separator (C++14 syntax was integer-only; in raw UDLs the
 * separator can show up regardless of standard). */
static_assert(1010'1100_bin == 172ULL, "1010'1100_bin == 172 (0xAC)");
static_assert(0'0'0_bin == 0ULL, "all zeros with separators");

/* Powers of two. */
static_assert(100_bin == 4ULL, "100_bin == 4");
static_assert(1000_bin == 8ULL, "1000_bin == 8");
static_assert(10000_bin == 16ULL, "10000_bin == 16");
static_assert(1000000000_bin == 512ULL, "2^9");

TEST(udl_raw, zero)
{
    EXPECT_EQ(0_bin, 0ULL);
}

TEST(udl_raw, basic_values)
{
    EXPECT_EQ(1_bin, 1ULL);
    EXPECT_EQ(10_bin, 2ULL);
    EXPECT_EQ(11_bin, 3ULL);
    EXPECT_EQ(100_bin, 4ULL);
    EXPECT_EQ(101_bin, 5ULL);
}

TEST(udl_raw, eight_bit_max)
{
    EXPECT_EQ(11111111_bin, 255ULL);
}

TEST(udl_raw, digit_separators)
{
    EXPECT_EQ(1010'1100_bin, 172ULL); /* 0xAC */
    EXPECT_EQ(1'0'1'0_bin, 10ULL);
    EXPECT_EQ(11'11'11'11_bin, 255ULL);
}

TEST(udl_raw, larger_values)
{
    EXPECT_EQ(100000000_bin, 256ULL);
    EXPECT_EQ(1111111111111111_bin, 65535ULL);
}

TEST(udl_raw, return_type_is_unsigned_long_long)
{
    static_assert(std::is_same<decltype(1_bin), unsigned long long>::value, "_bin returns unsigned long long");
    SUCCEED();
}
