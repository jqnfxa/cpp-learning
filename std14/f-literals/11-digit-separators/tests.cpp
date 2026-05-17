#include <gtest/gtest.h>

#include <cstdint>

#include "solution.hpp"

namespace t = task11;

TEST(digit_separators, million_is_1e6)
{
    EXPECT_EQ(t::million, 1000000);
}

TEST(digit_separators, billion_is_1e9)
{
    EXPECT_EQ(t::billion, 1000000000LL);
}

TEST(digit_separators, billion_fits_long_long)
{
    /* Belt-and-braces: 1e9 is in int range, but we wrote LL to
     * be explicit. Pin the type. */
    EXPECT_TRUE((std::is_same<decltype(t::billion), const long long>::value));
}

TEST(digit_separators, mac_prefix_is_AABBCCDD)
{
    EXPECT_EQ(t::mac_prefix, 0xAABBCCDDu);
}

TEST(digit_separators, mac_prefix_first_byte_is_AA)
{
    EXPECT_EQ((t::mac_prefix >> 24) & 0xFF, 0xAAu);
}

TEST(digit_separators, subnet_value_is_49320)
{
    EXPECT_EQ(t::subnet, 49320u);
}

TEST(digit_separators, subnet_high_byte_is_C0)
{
    EXPECT_EQ((t::subnet >> 8) & 0xFF, 0xC0u);
}

TEST(digit_separators, subnet_low_byte_is_A8)
{
    EXPECT_EQ(t::subnet & 0xFF, 0xA8u);
}

TEST(digit_separators, pi_micro_is_3141592)
{
    EXPECT_EQ(t::pi_micro, 3141592);
}

TEST(digit_separators, million_evaluable_at_compile_time)
{
    constexpr int m = t::million;
    EXPECT_EQ(m, 1000000);
}
