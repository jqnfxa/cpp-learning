#include <gtest/gtest.h>

#include "solution.hpp"

/* Compile-time pins. */
static_assert(task46::calling_safe_is_noexcept(), "safe() is noexcept");
static_assert(!task46::calling_unsafe_is_noexcept(), "unsafe() is NOT noexcept");
static_assert(task46::int_default_init_is_noexcept(), "int{} is noexcept");
static_assert(!task46::new_int_is_noexcept(), "new int(0) is NOT noexcept (operator new may throw)");

/*
 * Composite: noexcept on a comma-expression returns true iff every
 * subexpression is noexcept. safe() is, unsafe() is not -> false.
 */
static_assert(!noexcept(task46::safe(), task46::unsafe()), "composite is not noexcept if any part isn't");

/*
 * Unevaluated operand: the expression inside noexcept(...) is never
 * actually evaluated. We can write a deliberately nonsense
 * expression that would crash if executed, and the noexcept query
 * just inspects its type-level noexcept-ness.
 */
static_assert(noexcept(*static_cast<int *>(nullptr)), "deref-of-null-int -- unevaluated, doesn't crash");

TEST(noexcept_op, runtime_returns)
{
    EXPECT_TRUE(task46::calling_safe_is_noexcept());
    EXPECT_FALSE(task46::calling_unsafe_is_noexcept());
    EXPECT_TRUE(task46::int_default_init_is_noexcept());
    EXPECT_FALSE(task46::new_int_is_noexcept());
}

TEST(noexcept_op, nothrow_new_is_noexcept)
{
    /* The nothrow placement form of new doesn't throw; it returns
     * nullptr on failure. So noexcept(new (std::nothrow) int(0))
     * is true. */
    EXPECT_TRUE(noexcept(new (std::nothrow) int(0)));
}
