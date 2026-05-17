#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task18;

TEST(u8_char_literal, letter_a_is_65)
{
    EXPECT_EQ(t::letter_a(), 'A');
    EXPECT_EQ(t::letter_a(), 65);
}

TEST(u8_char_literal, digit_zero_is_48)
{
    EXPECT_EQ(t::digit_zero(), '0');
    EXPECT_EQ(t::digit_zero(), 48);
}

TEST(u8_char_literal, newline_is_10)
{
    EXPECT_EQ(t::newline(), '\n');
    EXPECT_EQ(t::newline(), 10);
}

TEST(u8_char_literal, evaluable_at_compile_time)
{
    constexpr char c = t::letter_a();
    EXPECT_EQ(c, 'A');
}
