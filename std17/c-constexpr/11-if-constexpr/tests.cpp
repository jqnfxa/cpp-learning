#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

namespace t = task11;

TEST(if_constexpr, describe_int)
{
    EXPECT_EQ(t::describe(42), "int:42");
}

TEST(if_constexpr, describe_negative_int)
{
    EXPECT_EQ(t::describe(-7), "int:-7");
}

TEST(if_constexpr, describe_long)
{
    EXPECT_EQ(t::describe(100L), "int:100");
}

TEST(if_constexpr, describe_double_starts_with_float)
{
    auto s = t::describe(3.14);
    /* std::to_string(double) may produce "3.140000" -- only
     * the "float:" prefix is reliable. */
    EXPECT_EQ(s.substr(0, 6), "float:");
}

TEST(if_constexpr, describe_other)
{
    /* A pointer is neither integral nor floating-point.
     * (Well, an int* pointer is "not arithmetic" per the
     * trait.) */
    int x = 0;
    int *p = &x;
    EXPECT_EQ(t::describe(p), "other");
}

TEST(if_constexpr, size_of_string)
{
    std::string s = "hello";
    EXPECT_EQ(t::size_of(s), 5u);
}

TEST(if_constexpr, size_of_empty_string)
{
    std::string s;
    EXPECT_EQ(t::size_of(s), 0u);
}

TEST(if_constexpr, size_of_int)
{
    EXPECT_EQ(t::size_of(42), 1u);
}

TEST(if_constexpr, size_of_double)
{
    EXPECT_EQ(t::size_of(3.14), 1u);
}

TEST(if_constexpr, size_of_pointer)
{
    int x = 0;
    int *p = &x;
    EXPECT_EQ(t::size_of(p), 0u);
}
