#include <gtest/gtest.h>

#include <variant>

#include "solution.hpp"

namespace t = task30;

TEST(variant, null_index_is_0)
{
    auto v = t::make_null();
    EXPECT_EQ(v.index(), 0u);
}

TEST(variant, bool_index_is_1)
{
    auto v = t::make_bool(true);
    EXPECT_EQ(v.index(), 1u);
    EXPECT_EQ(std::get<bool>(v), true);
}

TEST(variant, double_index_is_2)
{
    auto v = t::make_number(3.14);
    EXPECT_EQ(v.index(), 2u);
    EXPECT_DOUBLE_EQ(std::get<double>(v), 3.14);
}

TEST(variant, string_index_is_3)
{
    auto v = t::make_string("hello");
    EXPECT_EQ(v.index(), 3u);
    EXPECT_EQ(std::get<std::string>(v), "hello");
}

TEST(variant, holds_alternative)
{
    auto v = t::make_bool(false);
    EXPECT_TRUE(std::holds_alternative<bool>(v));
    EXPECT_FALSE(std::holds_alternative<double>(v));
}
