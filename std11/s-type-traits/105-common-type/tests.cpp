#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

/* Return-type pins. */
static_assert(std::is_same<decltype(task105::max(1)), int>::value,
              "max(int) -> int");
static_assert(std::is_same<decltype(task105::max(1, 2)), int>::value,
              "max(int, int) -> int");
static_assert(std::is_same<decltype(task105::max(1, 2.0)), double>::value,
              "max(int, double) -> double");
static_assert(std::is_same<decltype(task105::max(1, 2, 3)), int>::value,
              "max(int, int, int) -> int");
static_assert(std::is_same<decltype(task105::max(1, 2.0, 3L)), double>::value,
              "max(int, double, long) -> double");

TEST(common_type, single_arg)
{
    EXPECT_EQ(task105::max(42), 42);
    EXPECT_EQ(task105::max(-1), -1);
    EXPECT_EQ(task105::max(0), 0);
}

TEST(common_type, two_args_same_type)
{
    EXPECT_EQ(task105::max(1, 2), 2);
    EXPECT_EQ(task105::max(2, 1), 2);
    EXPECT_EQ(task105::max(5, 5), 5);
}

TEST(common_type, two_args_mixed_promotes)
{
    EXPECT_DOUBLE_EQ(task105::max(1, 2.5), 2.5);
    EXPECT_DOUBLE_EQ(task105::max(2.5, 1), 2.5);
    EXPECT_DOUBLE_EQ(task105::max(3.0, 1), 3.0);
}

TEST(common_type, three_args)
{
    EXPECT_EQ(task105::max(1, 2, 3), 3);
    EXPECT_EQ(task105::max(3, 2, 1), 3);
    EXPECT_EQ(task105::max(2, 3, 1), 3);
}

TEST(common_type, many_args)
{
    EXPECT_EQ(task105::max(5, 3, 1, 8, 4, 2), 8);
    EXPECT_EQ(task105::max(1, 2, 3, 4, 5, 6, 7), 7);
    EXPECT_EQ(task105::max(-5, -3, -1, -8, -4, -2), -1);
}

TEST(common_type, mixed_promotion_three_args)
{
    /* common_type<int, double, long> == double; max is 3.0L coerced
     * to double. */
    EXPECT_DOUBLE_EQ(task105::max(1, 2.0, 3L), 3.0);
}

TEST(common_type, longest_chain)
{
    /* Eight arguments. Tests the recursion holds together. */
    EXPECT_EQ(task105::max(1, 2, 3, 4, 5, 6, 7, 100), 100);
}
