#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task20;

TEST(nodiscard, must_read_returns_doubled)
{
    EXPECT_EQ(t::must_read(5), 10);
}

TEST(nodiscard, must_read_zero)
{
    EXPECT_EQ(t::must_read(0), 0);
}

TEST(nodiscard, must_read_negative)
{
    EXPECT_EQ(t::must_read(-7), -14);
}

TEST(nodiscard, parse_returns_tripled)
{
    auto ec = t::parse(7);
    EXPECT_EQ(ec.value, 21);
}

TEST(nodiscard, parse_zero)
{
    auto ec = t::parse(0);
    EXPECT_EQ(ec.value, 0);
}
