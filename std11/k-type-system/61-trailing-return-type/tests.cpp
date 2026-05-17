#include <gtest/gtest.h>

#include <type_traits>
#include <vector>

#include "solution.hpp"

/* Return-type pins via static_assert. */
static_assert(std::is_same<decltype(task61::add(1, 2)), int>::value, "add(int, int) -> int");
static_assert(std::is_same<decltype(task61::add(1, 2.5)), double>::value, "add(int, double) -> double");
static_assert(std::is_same<decltype(task61::multiply(2, 3)), int>::value, "multiply(int, int) -> int");
static_assert(std::is_same<decltype(task61::multiply(2, 1.5)), double>::value, "multiply(int, double) -> double");

TEST(trailing_return, add_ints)
{
    EXPECT_EQ(task61::add(1, 2), 3);
    EXPECT_EQ(task61::add(-3, 5), 2);
}

TEST(trailing_return, add_mixed_types)
{
    EXPECT_DOUBLE_EQ(task61::add(1, 2.5), 3.5);
    EXPECT_DOUBLE_EQ(task61::add(1.5, 1), 2.5);
}

TEST(trailing_return, multiply_ints)
{
    EXPECT_EQ(task61::multiply(2, 3), 6);
    EXPECT_EQ(task61::multiply(-2, 5), -10);
}

TEST(trailing_return, multiply_mixed_types)
{
    EXPECT_DOUBLE_EQ(task61::multiply(2, 1.5), 3.0);
    EXPECT_DOUBLE_EQ(task61::multiply(0.5, 0.5), 0.25);
}

TEST(trailing_return, first_element_int_vector)
{
    std::vector<int> v = {10, 20, 30};
    EXPECT_EQ(task61::first_element(v), 10);
}

TEST(trailing_return, first_element_double_vector)
{
    std::vector<double> v = {3.14, 2.71};
    EXPECT_DOUBLE_EQ(task61::first_element(v), 3.14);
}

TEST(trailing_return, first_element_single)
{
    std::vector<int> v = {42};
    EXPECT_EQ(task61::first_element(v), 42);
}
