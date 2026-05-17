#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task31;

TEST(visit, int_renders)
{
    EXPECT_EQ(t::render(t::value{42}), "int:42");
}

TEST(visit, string_renders)
{
    EXPECT_EQ(t::render(t::value{std::string("hi")}), "str:hi");
}

TEST(visit, negative_int)
{
    EXPECT_EQ(t::render(t::value{-7}), "int:-7");
}

TEST(visit, empty_string)
{
    EXPECT_EQ(t::render(t::value{std::string("")}), "str:");
}
