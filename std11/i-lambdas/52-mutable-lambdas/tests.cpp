#include <gtest/gtest.h>

#include "solution.hpp"

TEST(mutable_lambdas, counter_from_zero)
{
    auto c = task52::make_counter(0);
    EXPECT_EQ(c(), 1);
    EXPECT_EQ(c(), 2);
    EXPECT_EQ(c(), 3);
    EXPECT_EQ(c(), 4);
    EXPECT_EQ(c(), 5);
}

TEST(mutable_lambdas, counter_from_ten)
{
    auto c = task52::make_counter(10);
    EXPECT_EQ(c(), 11);
    EXPECT_EQ(c(), 12);
    EXPECT_EQ(c(), 13);
}

TEST(mutable_lambdas, counter_from_negative)
{
    auto c = task52::make_counter(-5);
    EXPECT_EQ(c(), -4);
    EXPECT_EQ(c(), -3);
    EXPECT_EQ(c(), -2);
    EXPECT_EQ(c(), -1);
    EXPECT_EQ(c(), 0);
    EXPECT_EQ(c(), 1);
}

TEST(mutable_lambdas, counters_are_independent)
{
    auto a = task52::make_counter(0);
    auto b = task52::make_counter(0);

    EXPECT_EQ(a(), 1);
    EXPECT_EQ(a(), 2);
    EXPECT_EQ(a(), 3);
    /* `b` should still be at its initial state. */
    EXPECT_EQ(b(), 1);
    EXPECT_EQ(b(), 2);

    /* `a` keeps moving. */
    EXPECT_EQ(a(), 4);
    EXPECT_EQ(b(), 3);
}

TEST(mutable_lambdas, many_calls)
{
    auto c = task52::make_counter(0);
    for (int i = 1; i <= 100; ++i)
    {
        EXPECT_EQ(c(), i);
    }
}
