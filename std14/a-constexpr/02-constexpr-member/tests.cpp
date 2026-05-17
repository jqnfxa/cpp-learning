#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task02;

TEST(constexpr_member, default_construction_is_zero)
{
    t::point p;
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
}

TEST(constexpr_member, explicit_ctor_assigns)
{
    t::point p(3, 4);
    EXPECT_EQ(p.x, 3);
    EXPECT_EQ(p.y, 4);
}

TEST(constexpr_member, set_x_mutates_x_only)
{
    t::point p(1, 2);
    p.set_x(99);
    EXPECT_EQ(p.x, 99);
    EXPECT_EQ(p.y, 2);
}

TEST(constexpr_member, set_y_mutates_y_only)
{
    t::point p(1, 2);
    p.set_y(99);
    EXPECT_EQ(p.x, 1);
    EXPECT_EQ(p.y, 99);
}

TEST(constexpr_member, multiple_setters_last_wins)
{
    t::point p;
    p.set_x(1);
    p.set_x(2);
    p.set_x(3);
    EXPECT_EQ(p.x, 3);
}

TEST(constexpr_member, make_point_runtime)
{
    t::point p = t::make_point(5, 7);
    EXPECT_EQ(p.x, 5);
    EXPECT_EQ(p.y, 7);
}

TEST(constexpr_member, make_point_at_compile_time)
{
    /* The C++14 promise: a constexpr SETTER works inside a
     * constexpr function. The `constexpr point` variable
     * forces compile-time evaluation -- if `set_x` weren't
     * legal in constexpr context, this would fail to compile. */
    constexpr t::point p = t::make_point(11, 22);
    EXPECT_EQ(p.x, 11);
    EXPECT_EQ(p.y, 22);
}

TEST(constexpr_member, compile_time_zero_point)
{
    constexpr t::point p = t::make_point(0, 0);
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
}

TEST(constexpr_member, compile_time_negative)
{
    constexpr t::point p = t::make_point(-3, -7);
    EXPECT_EQ(p.x, -3);
    EXPECT_EQ(p.y, -7);
}

TEST(constexpr_member, getters_are_const)
{
    /* Sanity: the getters are explicitly const and work on
     * const points. (The setters are NOT const -- the whole
     * point of C++14 relaxation.) */
    const t::point p(7, 8);
    EXPECT_EQ(p.get_x(), 7);
    EXPECT_EQ(p.get_y(), 8);
}
