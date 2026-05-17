#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>

#include "solution.hpp"

namespace t = task25;

TEST(exception_specs, may_throw_returns_42)
{
    EXPECT_EQ(t::may_throw(false), 42);
}

TEST(exception_specs, may_throw_throws)
{
    EXPECT_THROW(t::may_throw(true), std::runtime_error);
}

TEST(exception_specs, wont_throw_returns_99)
{
    EXPECT_EQ(t::wont_throw(), 99);
}

TEST(exception_specs, wont_throw_is_noexcept)
{
    EXPECT_TRUE(noexcept(t::wont_throw()));
}

TEST(exception_specs, may_throw_is_not_noexcept)
{
    EXPECT_FALSE(noexcept(t::may_throw(false)));
}
