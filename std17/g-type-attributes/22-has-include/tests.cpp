#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task22;

TEST(has_include, optional_is_present)
{
    EXPECT_TRUE(t::has_optional());
}

TEST(has_include, nonexistent_header_is_false)
{
    EXPECT_FALSE(t::has_nonexistent_header());
}

TEST(has_include, optional_value_42)
{
    auto o = t::make_optional_int(42);
    ASSERT_TRUE(o.has_value());
    EXPECT_EQ(*o, 42);
}

TEST(has_include, optional_value_zero)
{
    auto o = t::make_optional_int(0);
    ASSERT_TRUE(o.has_value());
    EXPECT_EQ(*o, 0);
}
