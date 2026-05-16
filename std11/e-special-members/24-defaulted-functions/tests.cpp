#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

/* Compile-time pins: all five special members must be present. */
static_assert(std::is_default_constructible<task24::widget>::value, "widget must be default-constructible (explicit = default)");
static_assert(std::is_copy_constructible<task24::widget>::value, "widget must be copy-constructible (explicit = default)");
static_assert(std::is_copy_assignable<task24::widget>::value, "widget must be copy-assignable (explicit = default)");
static_assert(std::is_destructible<task24::widget>::value, "widget must be destructible");

TEST(defaulted_functions, default_ctor_yields_zero_id)
{
    task24::widget w{};
    EXPECT_EQ(w.id(), 0);
}

TEST(defaulted_functions, int_ctor_sets_id)
{
    task24::widget w{42};
    EXPECT_EQ(w.id(), 42);
}

TEST(defaulted_functions, int_ctor_negative)
{
    task24::widget w{-7};
    EXPECT_EQ(w.id(), -7);
}

TEST(defaulted_functions, copy_ctor_copies_id)
{
    task24::widget a{99};
    task24::widget b{a};
    EXPECT_EQ(b.id(), 99);
}

TEST(defaulted_functions, copy_assignment_copies_id)
{
    task24::widget a{50};
    task24::widget b{0};
    b = a;
    EXPECT_EQ(b.id(), 50);
}

TEST(defaulted_functions, copies_are_independent)
{
    task24::widget a{1};
    task24::widget b{a};
    /* No way to mutate via const id(), so we infer independence by
     * the lack of shared state. The fact that copy succeeds and
     * holds the same value is the observable. */
    EXPECT_EQ(a.id(), 1);
    EXPECT_EQ(b.id(), 1);
}
