#include <gtest/gtest.h>
#include <any>

#include "solution.hpp"

namespace t = task32;

TEST(any, wrap_int_holds_value)
{
    auto a = t::wrap_int(42);
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(std::any_cast<int>(a), 42);
}

TEST(any, wrap_string_holds_value)
{
    auto a = t::wrap_string("hello");
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(std::any_cast<std::string>(a), "hello");
}

TEST(any, unwrap_int)
{
    EXPECT_EQ(t::unwrap_int(t::wrap_int(7)), 7);
}

TEST(any, wrong_cast_throws)
{
    auto a = t::wrap_int(42);
    EXPECT_THROW(std::any_cast<double>(a), std::bad_any_cast);
}
