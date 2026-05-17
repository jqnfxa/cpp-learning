#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task28;

TEST(optional, parse_valid)
{
    auto v = t::parse_int("42");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(optional, parse_negative)
{
    auto v = t::parse_int("-7");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, -7);
}

TEST(optional, parse_invalid_empty)
{
    auto v = t::parse_int("");
    EXPECT_FALSE(v.has_value());
}

TEST(optional, parse_invalid_letters)
{
    auto v = t::parse_int("abc");
    EXPECT_FALSE(v.has_value());
}

TEST(optional, parse_zero)
{
    auto v = t::parse_int("0");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 0);
}
