#include <gtest/gtest.h>

#include "solution.hpp"

TEST(delegating_ctors, default_ctor_zero_zero)
{
    task22::point p;
    EXPECT_EQ(p.x(), 0);
    EXPECT_EQ(p.y(), 0);
}

TEST(delegating_ctors, single_int_ctor_copies_to_both)
{
    task22::point p{5};
    EXPECT_EQ(p.x(), 5);
    EXPECT_EQ(p.y(), 5);
}

TEST(delegating_ctors, single_int_ctor_with_negative)
{
    task22::point p{-7};
    EXPECT_EQ(p.x(), -7);
    EXPECT_EQ(p.y(), -7);
}

TEST(delegating_ctors, two_arg_ctor_distinct_values)
{
    task22::point p{3, 4};
    EXPECT_EQ(p.x(), 3);
    EXPECT_EQ(p.y(), 4);
}

TEST(delegating_ctors, two_arg_ctor_zero_y)
{
    task22::point p{10, 0};
    EXPECT_EQ(p.x(), 10);
    EXPECT_EQ(p.y(), 0);
}

TEST(delegating_ctors, multiple_instances_independent)
{
    task22::point a{1, 2};
    task22::point b{};
    task22::point c{99};

    EXPECT_EQ(a.x(), 1);
    EXPECT_EQ(a.y(), 2);
    EXPECT_EQ(b.x(), 0);
    EXPECT_EQ(b.y(), 0);
    EXPECT_EQ(c.x(), 99);
    EXPECT_EQ(c.y(), 99);
}
