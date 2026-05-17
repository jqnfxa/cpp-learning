#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task19;

TEST(fallthrough, zero_falls_through_to_small)
{
    EXPECT_EQ(t::classify(0), "zero small");
}

TEST(fallthrough, one_is_small)
{
    EXPECT_EQ(t::classify(1), "small");
}

TEST(fallthrough, two_is_small)
{
    EXPECT_EQ(t::classify(2), "small");
}

TEST(fallthrough, three_is_three)
{
    EXPECT_EQ(t::classify(3), "three");
}

TEST(fallthrough, other_is_other)
{
    EXPECT_EQ(t::classify(99), "other");
}

TEST(fallthrough, negative_is_other)
{
    EXPECT_EQ(t::classify(-5), "other");
}
