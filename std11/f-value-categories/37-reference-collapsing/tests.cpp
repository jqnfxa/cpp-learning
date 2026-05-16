#include <gtest/gtest.h>

#include <cstring>
#include <type_traits>
#include <utility>

#include "solution.hpp"

/*
 * Pin the four reference-collapsing rules at compile time. Substitute
 * T && for each spelling of T and verify it collapses to the right
 * thing.
 */

/* T = int  -> int && stays int && (no collapsing). */
static_assert(std::is_same<int &&, int &&>::value, "T=int -> T && is int &&");

/* The four interesting cases via template aliases that force the substitution. */
template <class T>
using param_type = T &&;

static_assert(std::is_same<param_type<int>, int &&>::value, "T=int -> int &&");
static_assert(std::is_same<param_type<int &>, int &>::value, "T=int & -> (int & &&) collapses to int &");
static_assert(std::is_same<param_type<int &&>, int &&>::value, "T=int && -> (int && &&) collapses to int &&");

/* And the const-qualified version: T=const int & still collapses to const int &. */
static_assert(std::is_same<param_type<const int &>, const int &>::value, "T=const int & -> collapses to const int &");

TEST(reference_collapsing, deduce_lvalue_gives_lvalue_ref)
{
    int x = 0;
    EXPECT_STREQ(task37::deduce(x), "lvalue_ref");
}

TEST(reference_collapsing, deduce_literal_gives_value)
{
    EXPECT_STREQ(task37::deduce(42), "value");
}

TEST(reference_collapsing, deduce_std_move_gives_value)
{
    int x = 0;
    EXPECT_STREQ(task37::deduce(std::move(x)), "value");
}

TEST(reference_collapsing, deduce_named_rvalue_reference_gives_lvalue_ref)
{
    /*
     * `rr`'s declared type is int && but its expression category is
     * lvalue. So deduce(rr) deduces T as int & -- exactly the same
     * collapse-to-lvalue-ref path as a plain int variable. This is
     * the connection to step 30's named-T&&-is-lvalue lesson.
     */
    int &&rr = 7;
    EXPECT_STREQ(task37::deduce(rr), "lvalue_ref");
}

TEST(reference_collapsing, deduce_temporary_object_gives_value)
{
    EXPECT_STREQ(task37::deduce(int{}), "value");
}

TEST(reference_collapsing, kind_directly_on_each_spelling)
{
    /*
     * kind<T>() is independent of template deduction: it just
     * inspects T. Spell each variant explicitly. The
     * rvalue-ref branch is unreachable through deduce() but
     * reachable here.
     */
    EXPECT_STREQ(task37::kind<int>(), "value");
    EXPECT_STREQ(task37::kind<int &>(), "lvalue_ref");
    EXPECT_STREQ(task37::kind<int &&>(), "rvalue_ref");
}
