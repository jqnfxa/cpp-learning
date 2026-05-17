#include <gtest/gtest.h>

#include "solution.hpp"

TEST(lambda_to_fnptr, get_doubler_basic)
{
    auto d = task53::get_doubler();
    ASSERT_NE(d, nullptr);
    EXPECT_EQ(d(3), 6);
    EXPECT_EQ(d(0), 0);
    EXPECT_EQ(d(-5), -10);
}

TEST(lambda_to_fnptr, get_doubler_returns_function_pointer_type)
{
    /* Pin the return type at compile time. */
    int (*d)(int) = task53::get_doubler();
    static_assert(std::is_same<decltype(task53::get_doubler()), int (*)(int)>::value, "get_doubler returns int(*)(int)");
    ASSERT_NE(d, nullptr);
    EXPECT_EQ(d(7), 14);
}

TEST(lambda_to_fnptr, apply_with_get_doubler)
{
    EXPECT_EQ(task53::apply(task53::get_doubler(), 7), 14);
    EXPECT_EQ(task53::apply(task53::get_doubler(), 0), 0);
}

TEST(lambda_to_fnptr, apply_with_inline_lambda)
{
    /* The inline lambda is captureless, so it implicitly converts
     * to int(*)(int) at the call site. */
    EXPECT_EQ(task53::apply([](int x)
                            { return x + 1; }, 9),
              10);
    EXPECT_EQ(task53::apply([](int x)
                            { return -x; }, 42),
              -42);
    EXPECT_EQ(task53::apply([](int x)
                            { return x * x; }, 5),
              25);
}

TEST(lambda_to_fnptr, apply_with_named_function)
{
    /* Plain old function pointer also works (same type). */
    struct local
    {
        static int triple(int x)
        {
            return x * 3;
        }
    };
    EXPECT_EQ(task53::apply(&local::triple, 4), 12);
}
