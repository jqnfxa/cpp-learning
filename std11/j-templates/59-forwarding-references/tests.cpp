#include <gtest/gtest.h>

#include <utility>

#include "solution.hpp"

TEST(forwarding_refs, category_lvalue_int)
{
    int x = 0;
    EXPECT_STREQ(task59::category(x), "lvalue");
}

TEST(forwarding_refs, category_rvalue_literal)
{
    EXPECT_STREQ(task59::category(42), "rvalue");
}

TEST(forwarding_refs, category_std_move)
{
    int x = 0;
    EXPECT_STREQ(task59::category(std::move(x)), "rvalue");
}

TEST(forwarding_refs, category_named_rvalue_reference_is_lvalue)
{
    /* Named T && variable is itself an lvalue when used. */
    int &&rr = 1;
    EXPECT_STREQ(task59::category(rr), "lvalue");
    EXPECT_STREQ(task59::category(std::move(rr)), "rvalue");
}

TEST(forwarding_refs, deduced_kind_lvalue)
{
    int x = 0;
    EXPECT_STREQ(task59::deduced_kind(x), "T_amp");
}

TEST(forwarding_refs, deduced_kind_rvalue)
{
    EXPECT_STREQ(task59::deduced_kind(42), "T");
    int x = 0;
    EXPECT_STREQ(task59::deduced_kind(std::move(x)), "T");
}

TEST(forwarding_refs, deduced_kind_temporary)
{
    EXPECT_STREQ(task59::deduced_kind(int{}), "T");
}

TEST(forwarding_refs, double_lvalue)
{
    double d = 0.0;
    EXPECT_STREQ(task59::category(d), "lvalue");
    EXPECT_STREQ(task59::deduced_kind(d), "T_amp");
}

TEST(forwarding_refs, string_literal_is_lvalue)
{
    /* A string literal "hello" is an lvalue (array). Passed to a
     * forwarding ref, T deduces to const char (&)[6], a reference
     * type -- so category reports lvalue. */
    EXPECT_STREQ(task59::category("hello"), "lvalue");
}
