#include <gtest/gtest.h>

#include "solution.hpp"

TEST(carries_dependency, get_token_returns_non_null)
{
    EXPECT_NE(task11::get_token(), nullptr);
}

TEST(carries_dependency, get_token_value_is_42)
{
    const int *p = task11::get_token();
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(*p, 42);
}

TEST(carries_dependency, get_token_address_is_stable)
{
    /* A function-local static has a single, stable address across
     * calls. Two separate calls should return the same pointer. */
    EXPECT_EQ(task11::get_token(), task11::get_token());
}

TEST(carries_dependency, consume_token_dereferences)
{
    int x = 7;
    EXPECT_EQ(task11::consume_token(&x), 7);
    x = 100;
    EXPECT_EQ(task11::consume_token(&x), 100);
}

TEST(carries_dependency, round_trip_get_then_consume)
{
    EXPECT_EQ(task11::consume_token(task11::get_token()), 42);
}
