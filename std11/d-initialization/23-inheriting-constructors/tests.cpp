#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "solution.hpp"

/* Inheritance relationship is pinned at compile time. */
static_assert(std::is_base_of<task23::base, task23::derived>::value, "derived publicly inherits from base");

TEST(inheriting_ctors, default_constructed_uses_base_default)
{
    task23::derived d{};
    EXPECT_EQ(d.value(), 0);
    EXPECT_EQ(d.name(), "default");
}

TEST(inheriting_ctors, single_int_uses_base_int_ctor)
{
    task23::derived d{42};
    EXPECT_EQ(d.value(), 42);
    EXPECT_EQ(d.name(), "default");
}

TEST(inheriting_ctors, single_int_negative)
{
    task23::derived d{-17};
    EXPECT_EQ(d.value(), -17);
    EXPECT_EQ(d.name(), "default");
}

TEST(inheriting_ctors, two_args_use_base_two_arg_ctor)
{
    task23::derived d{42, "x"};
    EXPECT_EQ(d.value(), 42);
    EXPECT_EQ(d.name(), "x");
}

TEST(inheriting_ctors, two_args_with_long_name)
{
    task23::derived d{7, "hello world"};
    EXPECT_EQ(d.value(), 7);
    EXPECT_EQ(d.name(), "hello world");
}

TEST(inheriting_ctors, multiple_independent_instances)
{
    task23::derived a;
    task23::derived b{1};
    task23::derived c{2, "alpha"};

    EXPECT_EQ(a.value(), 0);
    EXPECT_EQ(a.name(), "default");
    EXPECT_EQ(b.value(), 1);
    EXPECT_EQ(b.name(), "default");
    EXPECT_EQ(c.value(), 2);
    EXPECT_EQ(c.name(), "alpha");
}

TEST(inheriting_ctors, derived_is_a_base)
{
    task23::derived d{99, "polymorphic"};
    /* Slicing-style upcast still gives access to base interface. */
    const task23::base &b = d;
    EXPECT_EQ(b.value(), 99);
    EXPECT_EQ(b.name(), "polymorphic");
}
