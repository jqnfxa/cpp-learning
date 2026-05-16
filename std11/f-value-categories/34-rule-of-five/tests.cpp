#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "solution.hpp"

/* Compile-time pins for all five special members. */
static_assert(std::is_destructible<task34::handle>::value, "destructible");
static_assert(std::is_copy_constructible<task34::handle>::value, "copy-constructible");
static_assert(std::is_copy_assignable<task34::handle>::value, "copy-assignable");
static_assert(std::is_move_constructible<task34::handle>::value, "move-constructible");
static_assert(std::is_move_assignable<task34::handle>::value, "move-assignable");
static_assert(std::is_nothrow_move_constructible<task34::handle>::value, "move-ctor must be noexcept");
static_assert(std::is_nothrow_move_assignable<task34::handle>::value, "move-assign must be noexcept");

TEST(rule_of_five, default_constructed_is_empty)
{
    task34::handle h;
    EXPECT_FALSE(h.has_value());
}

TEST(rule_of_five, value_constructed_holds_value)
{
    task34::handle h{42};
    ASSERT_TRUE(h.has_value());
    EXPECT_EQ(h.value(), 42);
}

TEST(rule_of_five, copy_ctor_deep_copies_value)
{
    task34::handle a{7};
    task34::handle b{a};

    ASSERT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 7);
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 7);
}

TEST(rule_of_five, copy_ctor_from_empty)
{
    task34::handle a;
    task34::handle b{a};

    EXPECT_FALSE(a.has_value());
    EXPECT_FALSE(b.has_value());
}

TEST(rule_of_five, copy_assign_releases_old_data)
{
    task34::handle a{1};
    task34::handle b{2};

    b = a;
    ASSERT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 1);
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 1);
}

TEST(rule_of_five, copy_self_assign_is_safe)
{
    task34::handle a{99};
    a = a;
    ASSERT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 99);
}

TEST(rule_of_five, move_ctor_transfers_ownership)
{
    task34::handle a{5};
    task34::handle b{std::move(a)};

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 5);
    EXPECT_FALSE(a.has_value());
}

TEST(rule_of_five, move_assign_transfers_ownership)
{
    task34::handle a{17};
    task34::handle b{100};

    b = std::move(a);
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 17);
    EXPECT_FALSE(a.has_value());
}

TEST(rule_of_five, move_self_assign_is_safe)
{
    task34::handle a{11};
    a = std::move(a);
    /* Defensive form keeps a valid; with self-guard it's unchanged. */
    ASSERT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 11);
}
