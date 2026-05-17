#include <gtest/gtest.h>

#include <functional>

#include "solution.hpp"

TEST(std_bind, member_basic)
{
    task95::Adder a{10};
    auto f = task95::bind_member(a);
    ASSERT_TRUE(static_cast<bool>(f));
    EXPECT_EQ(f(5), 15);
    EXPECT_EQ(f(-3), 7);
    EXPECT_EQ(f(0), 10);
}

TEST(std_bind, member_negative_base)
{
    task95::Adder a{-100};
    auto f = task95::bind_member(a);
    EXPECT_EQ(f(50), -50);
    EXPECT_EQ(f(100), 0);
    EXPECT_EQ(f(200), 100);
}

TEST(std_bind, member_sees_object_mutations)
{
    /* The bind captures `&a` (a pointer), so mutating the underlying
     * Adder's base is visible through the bound function. This is
     * what `&a` (vs. capturing the Adder by value) gives you. */
    task95::Adder a{1};
    auto f = task95::bind_member(a);
    EXPECT_EQ(f(10), 11);

    a.base = 100;
    EXPECT_EQ(f(10), 110);
}

TEST(std_bind, free_basic)
{
    auto f = task95::bind_free(7);
    ASSERT_TRUE(static_cast<bool>(f));
    EXPECT_EQ(f(3), 10);
    EXPECT_EQ(f(-7), 0);
    EXPECT_EQ(f(0), 7);
}

TEST(std_bind, free_zero_base)
{
    auto f = task95::bind_free(0);
    EXPECT_EQ(f(42), 42);
    EXPECT_EQ(f(-1), -1);
}

TEST(std_bind, member_and_free_agree)
{
    /* bind_member(Adder{k}) and bind_free(k) should be equivalent
     * functions for the same k. */
    task95::Adder a{20};
    auto m = task95::bind_member(a);
    auto fr = task95::bind_free(20);
    for (int x = -5; x <= 5; ++x)
    {
        EXPECT_EQ(m(x), fr(x)) << "x=" << x;
    }
}

TEST(std_bind, function_is_assignable_to_std_function)
{
    /* The signature says std::function<int(int)>, so the test verifies
     * the bind result actually fits in that slot (which is the whole
     * pedagogical point of pairing bind with function). */
    std::function<int(int)> stored = task95::bind_free(100);
    EXPECT_EQ(stored(1), 101);
}
