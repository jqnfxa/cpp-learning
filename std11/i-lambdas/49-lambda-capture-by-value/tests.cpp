#include <gtest/gtest.h>

#include "solution.hpp"

TEST(capture_by_value, make_constant_returns_value)
{
    auto f = task49::make_constant(42);
    EXPECT_EQ(f(), 42);
}

TEST(capture_by_value, make_constant_zero)
{
    auto f = task49::make_constant(0);
    EXPECT_EQ(f(), 0);
}

TEST(capture_by_value, make_constant_negative)
{
    auto f = task49::make_constant(-7);
    EXPECT_EQ(f(), -7);
}

TEST(capture_by_value, make_adder_basic)
{
    auto add5 = task49::make_adder(5);
    EXPECT_EQ(add5(3), 8);
    EXPECT_EQ(add5(0), 5);
    EXPECT_EQ(add5(-1), 4);
}

TEST(capture_by_value, make_adder_zero)
{
    auto identity = task49::make_adder(0);
    EXPECT_EQ(identity(100), 100);
    EXPECT_EQ(identity(-100), -100);
}

TEST(capture_by_value, make_adder_negative)
{
    auto sub1 = task49::make_adder(-1);
    EXPECT_EQ(sub1(10), 9);
    EXPECT_EQ(sub1(0), -1);
}

TEST(capture_by_value, multiple_instances_are_independent)
{
    auto add5 = task49::make_adder(5);
    auto add10 = task49::make_adder(10);
    EXPECT_EQ(add5(0), 5);
    EXPECT_EQ(add10(0), 10);
}

TEST(capture_by_value, snapshot_semantics_via_dangling_test)
{
    /*
     * If make_constant were buggy (captured by reference instead
     * of by value), the returned std::function would hold a
     * reference to the now-destroyed parameter `value`. Reading
     * f() would be undefined behavior.
     *
     * With a correct by-value capture, f() returns the snapshot
     * regardless of what happened to the original argument.
     */
    auto f = task49::make_constant(123);
    /* Force some unrelated work to (potentially) overwrite the
     * stack space where make_constant's parameter lived. */
    int local[64] = {};
    for (int i = 0; i < 64; ++i)
    {
        local[i] = i * 2;
    }
    (void)local;
    EXPECT_EQ(f(), 123);
}
