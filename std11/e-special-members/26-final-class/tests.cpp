#include <gtest/gtest.h>

#include "solution.hpp"

/*
 * Compile-time pin: `sealed` must be marked `final`, `openable` must
 * not be. __is_final is a compiler builtin -- std::is_final<T> is
 * C++14 and would not compile under -std=c++11.
 */
static_assert(__is_final(task26::sealed), "sealed must be marked `final`");
static_assert(!__is_final(task26::openable), "openable must NOT be marked `final`");

TEST(final_class, sealed_accessor_returns_42)
{
    task26::sealed s{};
    EXPECT_EQ(s.value(), 42);
}

TEST(final_class, openable_accessor_returns_7)
{
    task26::openable o{};
    EXPECT_EQ(o.value(), 7);
}

TEST(final_class, sealed_instances_are_independent)
{
    task26::sealed a{};
    task26::sealed b{};
    EXPECT_EQ(a.value(), 42);
    EXPECT_EQ(b.value(), 42);
}
