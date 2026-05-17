#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

namespace t = task08;

TEST(aggregate_defaults, point_default_init)
{
    t::point p{};
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
}

TEST(aggregate_defaults, point_no_init_uses_defaults)
{
    /* Without `{}` either: declaring a `point p;` should also
     * pick up the in-class member initializers. */
    t::point p;
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
}

TEST(aggregate_defaults, point_one_arg_y_defaults)
{
    /* Brace-init with one arg: x=1, y falls through to its
     * default. */
    t::point p{1};
    EXPECT_EQ(p.x, 1);
    EXPECT_EQ(p.y, 0);
}

TEST(aggregate_defaults, point_two_args_both_set)
{
    /* All explicit -- defaults overridden. */
    t::point p{1, 2};
    EXPECT_EQ(p.x, 1);
    EXPECT_EQ(p.y, 2);
}

TEST(aggregate_defaults, point_negative_values)
{
    t::point p{-3, -7};
    EXPECT_EQ(p.x, -3);
    EXPECT_EQ(p.y, -7);
}

TEST(aggregate_defaults, user_full_default)
{
    t::user u{};
    EXPECT_EQ(u.name, "anonymous");
    EXPECT_EQ(u.age, 0);
    EXPECT_TRUE(u.active);
}

TEST(aggregate_defaults, user_no_init)
{
    t::user u;
    EXPECT_EQ(u.name, "anonymous");
    EXPECT_EQ(u.age, 0);
    EXPECT_TRUE(u.active);
}

TEST(aggregate_defaults, user_with_name_only)
{
    t::user u{"alice"};
    EXPECT_EQ(u.name, "alice");
    EXPECT_EQ(u.age, 0);
    EXPECT_TRUE(u.active);
}

TEST(aggregate_defaults, user_with_name_and_age)
{
    t::user u{"alice", 30};
    EXPECT_EQ(u.name, "alice");
    EXPECT_EQ(u.age, 30);
    EXPECT_TRUE(u.active);
}

TEST(aggregate_defaults, user_with_all_fields)
{
    t::user u{"bob", 42, false};
    EXPECT_EQ(u.name, "bob");
    EXPECT_EQ(u.age, 42);
    EXPECT_FALSE(u.active);
}

TEST(aggregate_defaults, user_active_default_is_true)
{
    /* Pin the active default specifically -- the stub has it
     * false. */
    t::user u{"x", 1};
    EXPECT_TRUE(u.active);
}
