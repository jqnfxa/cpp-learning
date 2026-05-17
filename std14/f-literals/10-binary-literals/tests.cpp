#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task10;

TEST(binary_literals, mask_is_163)
{
    /* 0b1010'0011 = 128 + 32 + 2 + 1 = 163. */
    EXPECT_EQ(t::mask, 163);
}

TEST(binary_literals, all_ones_byte_is_255)
{
    EXPECT_EQ(t::all_ones_byte, 255);
}

TEST(binary_literals, alternating_is_85)
{
    /* 0b0101'0101 = 64 + 16 + 4 + 1 = 85. */
    EXPECT_EQ(t::alternating, 85);
}

TEST(binary_literals, mask_hex_equivalence)
{
    /* 0b1010'0011 == 0xA3. Pin the cross-base equality. */
    EXPECT_EQ(t::mask, 0xA3);
}

TEST(binary_literals, all_ones_hex_equivalence)
{
    EXPECT_EQ(t::all_ones_byte, 0xFF);
}

TEST(binary_literals, alternating_hex_equivalence)
{
    EXPECT_EQ(t::alternating, 0x55);
}

TEST(binary_literals, evaluable_at_compile_time)
{
    /* The constants are constexpr; usable in constant
     * expressions. */
    constexpr int m = t::mask;
    EXPECT_EQ(m, 163);
}

TEST(binary_literals, bitwise_and_works)
{
    /* 0b1010'0011 & 0b1111'1111 = 0b1010'0011. */
    EXPECT_EQ(t::mask & t::all_ones_byte, t::mask);
}

TEST(binary_literals, bitwise_xor_alternating)
{
    /* 0b0101'0101 ^ 0b1111'1111 = 0b1010'1010 = 170. */
    EXPECT_EQ(t::alternating ^ t::all_ones_byte, 0b1010'1010);
}
