#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task13;

TEST(capture_this, getter_initial_value)
{
    t::widget w(42);
    auto g = w.make_getter();
    EXPECT_EQ(g(), 42);
}

TEST(capture_this, getter_preserves_value_after_mutate)
{
    /* The KEY test: capture-by-value (*this) means the
     * closure has its OWN copy. Mutating the original after
     * capture does NOT affect the closure's view. With
     * pointer capture, the closure sees the modified value. */
    t::widget w(42);
    auto g = w.make_getter();
    w.set(999);
    EXPECT_EQ(g(), 42);
}

TEST(capture_this, getter_holds_value_independently)
{
    /* Multiple captures from the same object should be
     * independent snapshots. */
    t::widget w(10);
    auto g1 = w.make_getter();
    w.set(20);
    auto g2 = w.make_getter();
    w.set(30);
    EXPECT_EQ(g1(), 10);
    EXPECT_EQ(g2(), 20);
}

TEST(capture_this, multiple_independent_widgets)
{
    t::widget a(1);
    t::widget b(2);
    auto ga = a.make_getter();
    auto gb = b.make_getter();
    EXPECT_EQ(ga(), 1);
    EXPECT_EQ(gb(), 2);
}

TEST(capture_this, callable_repeatedly)
{
    t::widget w(7);
    auto g = w.make_getter();
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(g(), 7);
    }
}
