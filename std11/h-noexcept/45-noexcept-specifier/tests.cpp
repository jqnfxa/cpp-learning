#include <gtest/gtest.h>

#include "solution.hpp"

/*
 * Compile-time pins via the noexcept(expr) operator. This operator
 * (step 46) returns a bool constant telling you whether the given
 * expression is noexcept.
 *
 * Step 45 is about getting the *specifier on the declaration* right;
 * step 46 is the operator-side of this contract.
 */

static_assert(noexcept(task45::widget{}.clone()), "widget::clone() must be noexcept");
static_assert(!noexcept(task45::widget{}.compute()), "widget::compute() must NOT be noexcept");
static_assert(noexcept(task45::free_safe()), "free_safe() must be noexcept");
static_assert(!noexcept(task45::free_unsafe()), "free_unsafe() must NOT be noexcept");

TEST(noexcept_specifier, runtime_calls_return)
{
    /* Sanity check that the functions are callable. The return
     * values are unimportant; this exists so the suite has at
     * least one runtime test. */
    task45::widget w;
    EXPECT_EQ(w.clone(), 0);
    EXPECT_EQ(w.compute(), 0);
    EXPECT_EQ(task45::free_safe(), 0);
    EXPECT_EQ(task45::free_unsafe(), 0);
}

TEST(noexcept_specifier, runtime_noexcept_observability)
{
    /* The noexcept(...) operator is usable at runtime too -- the
     * result is just a compile-time constant promoted to an int. */
    task45::widget w;
    EXPECT_TRUE(noexcept(w.clone()));
    EXPECT_FALSE(noexcept(w.compute()));
    EXPECT_TRUE(noexcept(task45::free_safe()));
    EXPECT_FALSE(noexcept(task45::free_unsafe()));
}
