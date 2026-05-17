#include <gtest/gtest.h>

#include "solution.hpp"

TEST(variadic_recursion, format_single_int)
{
    EXPECT_EQ(task58::format(1), "1");
    EXPECT_EQ(task58::format(42), "42");
    EXPECT_EQ(task58::format(-7), "-7");
}

TEST(variadic_recursion, format_single_string)
{
    EXPECT_EQ(task58::format("hello"), "hello");
    EXPECT_EQ(task58::format(""), "");
}

TEST(variadic_recursion, format_single_float)
{
    EXPECT_EQ(task58::format(3.14), "3.14");
}

TEST(variadic_recursion, format_two_args)
{
    EXPECT_EQ(task58::format(1, 2), "1 2");
    EXPECT_EQ(task58::format("a", "b"), "a b");
}

TEST(variadic_recursion, format_three_args)
{
    EXPECT_EQ(task58::format(1, 2, 3), "1 2 3");
    EXPECT_EQ(task58::format("a", "b", "c"), "a b c");
}

TEST(variadic_recursion, format_many_mixed)
{
    EXPECT_EQ(task58::format(1, "+", 2, "=", 3), "1 + 2 = 3");
    EXPECT_EQ(task58::format("name:", "alice", "age:", 30), "name: alice age: 30");
}

TEST(variadic_recursion, format_long_sequence)
{
    EXPECT_EQ(task58::format(1, 2, 3, 4, 5, 6, 7), "1 2 3 4 5 6 7");
}
