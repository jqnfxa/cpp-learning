#include <gtest/gtest.h>

#include <utility>

#include "solution.hpp"

namespace
{

struct nothrow_movable
{
    nothrow_movable() = default;
    nothrow_movable(nothrow_movable &&) noexcept = default;
    nothrow_movable &operator=(nothrow_movable &&) noexcept = default;
};

struct throwing_movable
{
    throwing_movable() = default;

    /* Explicitly throwing -- noexcept(false). */
    throwing_movable(throwing_movable &&)
    {
    }

    throwing_movable &operator=(throwing_movable &&)
    {
        return *this;
    }
};

}

/* int -> noexcept. */
static_assert(noexcept(task47::swap_(std::declval<int &>(), std::declval<int &>())), "swap_<int> must be noexcept");

/* nothrow_movable -> noexcept. */
static_assert(noexcept(task47::swap_(std::declval<nothrow_movable &>(), std::declval<nothrow_movable &>())), "swap_<nothrow_movable> must be noexcept");

/* throwing_movable -> NOT noexcept. */
static_assert(!noexcept(task47::swap_(std::declval<throwing_movable &>(), std::declval<throwing_movable &>())), "swap_<throwing_movable> must NOT be noexcept");

TEST(conditional_noexcept, swap_int_is_noexcept)
{
    int a = 1;
    int b = 2;
    EXPECT_TRUE(noexcept(task47::swap_(a, b)));
}

TEST(conditional_noexcept, swap_nothrow_movable_is_noexcept)
{
    nothrow_movable a, b;
    EXPECT_TRUE(noexcept(task47::swap_(a, b)));
}

TEST(conditional_noexcept, swap_throwing_movable_is_not_noexcept)
{
    throwing_movable a, b;
    EXPECT_FALSE(noexcept(task47::swap_(a, b)));
}

TEST(conditional_noexcept, swap_int_actually_swaps)
{
    int a = 1;
    int b = 2;
    task47::swap_(a, b);
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 1);
}
