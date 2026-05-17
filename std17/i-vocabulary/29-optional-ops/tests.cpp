#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task29;

TEST(optional_ops, value_or_valid)
{
    EXPECT_EQ(t::parse_int_or("42", -1), 42);
}

TEST(optional_ops, value_or_invalid_returns_default)
{
    EXPECT_EQ(t::parse_int_or("abc", 99), 99);
}

TEST(optional_ops, value_or_empty_default)
{
    EXPECT_EQ(t::parse_int_or("", 0), 0);
}

TEST(optional_ops, doubled_valid)
{
    EXPECT_EQ(t::parse_int_doubled("7"), 14);
}

TEST(optional_ops, doubled_invalid_returns_0)
{
    EXPECT_EQ(t::parse_int_doubled("xyz"), 0);
}
