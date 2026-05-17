#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

namespace t = task03;

TEST(structured_bindings_structs, point_sum_simple)
{
    EXPECT_EQ(t::point_sum(t::make_point(3, 4)), 7);
}

TEST(structured_bindings_structs, point_sum_zero)
{
    /* Edge case: matches the stub's hardcoded 0. */
    EXPECT_EQ(t::point_sum(t::make_point(0, 0)), 0);
}

TEST(structured_bindings_structs, point_sum_negative)
{
    EXPECT_EQ(t::point_sum(t::make_point(-10, 3)), -7);
}

TEST(structured_bindings_structs, point_swap_basic)
{
    auto p = t::point_swap(t::make_point(1, 2));
    EXPECT_EQ(p.x, 2);
    EXPECT_EQ(p.y, 1);
}

TEST(structured_bindings_structs, point_swap_idempotent_pair)
{
    /* Swap twice gets back the original. */
    auto p = t::point_swap(t::point_swap(t::make_point(7, 99)));
    EXPECT_EQ(p.x, 7);
    EXPECT_EQ(p.y, 99);
}

TEST(structured_bindings_structs, profile_label_alice)
{
    auto s = t::profile_label(t::make_profile(30, "alice", 3.14));
    EXPECT_EQ(s, "alice:30");
}

TEST(structured_bindings_structs, profile_label_bob)
{
    auto s = t::profile_label(t::make_profile(42, "bob", 1.5));
    EXPECT_EQ(s, "bob:42");
}

TEST(structured_bindings_structs, profile_score_value)
{
    EXPECT_DOUBLE_EQ(
        t::profile_score(t::make_profile(1, "x", 9.5)), 9.5);
}

TEST(structured_bindings_structs, direct_point_binding)
{
    /* The textbook usage: bind a struct directly. */
    t::point p{10, 20};
    auto [a, b] = p;
    EXPECT_EQ(a, 10);
    EXPECT_EQ(b, 20);
}

TEST(structured_bindings_structs, direct_reference_binding)
{
    /* `auto& [a, b] = p;` aliases the struct's members. */
    t::point p{0, 0};
    auto &[a, b] = p;
    a = 1;
    b = 2;
    EXPECT_EQ(p.x, 1);
    EXPECT_EQ(p.y, 2);
}

TEST(structured_bindings_structs, binding_takes_declaration_order)
{
    /* The bindings follow the struct's DECLARATION order
     * (x first, then y). If the struct ever reordered, the
     * meaning of [a, b] would change. */
    t::point p{99, 1};
    auto [a, b] = p;
    EXPECT_EQ(a, 99); /* x first */
    EXPECT_EQ(b, 1);  /* then y */
}
