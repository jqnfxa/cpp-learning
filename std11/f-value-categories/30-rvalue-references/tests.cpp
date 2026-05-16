#include <gtest/gtest.h>

#include <utility>

#include "solution.hpp"

TEST(rvalue_refs, named_int_picks_lvalue_overload)
{
    int a = 0;
    EXPECT_EQ(task30::f(a), 1);
}

TEST(rvalue_refs, literal_picks_rvalue_overload)
{
    EXPECT_EQ(task30::f(7), 2);
}

TEST(rvalue_refs, std_move_picks_rvalue_overload)
{
    int a = 0;
    EXPECT_EQ(task30::f(std::move(a)), 2);
}

TEST(rvalue_refs, lvalue_reference_picks_lvalue_overload)
{
    int a = 0;
    int &r = a;
    EXPECT_EQ(task30::f(r), 1);
}

TEST(rvalue_refs, named_rvalue_reference_is_lvalue_when_used)
{
    /*
     * The gotcha. rr's *declared type* is int&&, but the *expression*
     * `rr` is an lvalue (a named entity). So f(rr) picks the lvalue
     * overload. To pick the rvalue overload, you have to std::move
     * the rvalue-reference back to xvalue form.
     */
    int &&rr = 0;
    EXPECT_EQ(task30::f(rr), 1);
    EXPECT_EQ(task30::f(std::move(rr)), 2);
}

TEST(rvalue_refs, temporary_picks_rvalue_overload)
{
    /* `int{}` is a prvalue. */
    EXPECT_EQ(task30::f(int{}), 2);
}
