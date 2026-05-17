#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "solution.hpp"

TEST(enable_if, to_int_from_int)
{
    EXPECT_EQ(task103::to_int(42), 42);
    EXPECT_EQ(task103::to_int(-17), -17);
    EXPECT_EQ(task103::to_int(0), 0);
}

TEST(enable_if, to_int_from_double_truncates)
{
    EXPECT_EQ(task103::to_int(3.14), 3);
    EXPECT_EQ(task103::to_int(3.99), 3);
    EXPECT_EQ(task103::to_int(-2.7), -2);
}

TEST(enable_if, to_int_from_char_is_ascii)
{
    EXPECT_EQ(task103::to_int('A'), 65);
    EXPECT_EQ(task103::to_int('0'), 48);
}

TEST(enable_if, to_int_from_bool)
{
    EXPECT_EQ(task103::to_int(true), 1);
    EXPECT_EQ(task103::to_int(false), 0);
}

TEST(enable_if, to_int_from_long)
{
    EXPECT_EQ(task103::to_int(123L), 123);
    EXPECT_EQ(task103::to_int(-1LL), -1);
}

TEST(enable_if, to_int_from_string)
{
    EXPECT_EQ(task103::to_int(std::string("123")), 123);
    EXPECT_EQ(task103::to_int(std::string("-42")), -42);
    EXPECT_EQ(task103::to_int(std::string("0")), 0);
}

TEST(enable_if, to_int_from_literal_string)
{
    /* "999" is const char[4], not std::string. Implicit conversion
     * to std::string lets the string overload pick it up. */
    EXPECT_EQ(task103::to_int(std::string("999")), 999);
}

TEST(enable_if, to_int_bad_string_throws)
{
    EXPECT_THROW(task103::to_int(std::string("not a number")), std::invalid_argument);
    EXPECT_THROW(task103::to_int(std::string("")), std::invalid_argument);
}

TEST(enable_if, to_int_partial_string_parses_prefix)
{
    /* std::stoi parses leading digits and stops. */
    EXPECT_EQ(task103::to_int(std::string("42abc")), 42);
    EXPECT_EQ(task103::to_int(std::string("  17 spaces")), 17);
}
