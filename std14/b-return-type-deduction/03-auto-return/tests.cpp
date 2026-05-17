#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "solution.hpp"

namespace t = task03;

TEST(auto_return, add_two_ints)
{
    EXPECT_EQ(t::add(2, 3), 5);
}

TEST(auto_return, add_mixed_int_double)
{
    EXPECT_DOUBLE_EQ(t::add(2, 3.5), 5.5);
}

TEST(auto_return, add_two_doubles)
{
    EXPECT_DOUBLE_EQ(t::add(1.25, 2.75), 4.0);
}

TEST(auto_return, multiply_two_ints)
{
    EXPECT_EQ(t::multiply(4, 5), 20);
}

TEST(auto_return, multiply_int_double)
{
    EXPECT_DOUBLE_EQ(t::multiply(2.0, 3), 6.0);
}

TEST(auto_return, subtract_two_ints)
{
    EXPECT_EQ(t::subtract(10, 3), 7);
}

TEST(auto_return, subtract_int_double_promotes)
{
    EXPECT_DOUBLE_EQ(t::subtract(10, 2.5), 7.5);
}

TEST(auto_return, add_strings_concatenates)
{
    /* string + const char* yields std::string. The deduction
     * picks up that std::string is the result type. */
    std::string hello = "hello";
    EXPECT_EQ(t::add(hello, " world"), std::string("hello world"));
}

TEST(auto_return, return_type_int_plus_double_is_double)
{
    /* The whole POINT of return-type deduction: the return
     * type tracks the expression's actual type, not the first
     * argument's type. With the stub (return a), the deduced
     * type is `int` and this assertion fails at runtime.
     *
     * Using EXPECT_TRUE rather than static_assert so the stub
     * still COMPILES -- the failure is caught at test time. */
    using R = decltype(t::add(1, 2.0));
    EXPECT_TRUE((std::is_same<R, double>::value));
}

TEST(auto_return, return_type_short_plus_int_is_int)
{
    /* Integer promotion: short + int -> int. */
    using R = decltype(t::add(short{1}, 2));
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(auto_return, return_type_multiply_int_long_is_long)
{
    using R = decltype(t::multiply(2, 3L));
    EXPECT_TRUE((std::is_same<R, long>::value));
}
