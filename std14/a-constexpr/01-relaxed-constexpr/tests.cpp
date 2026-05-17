#include <gtest/gtest.h>

#include "solution.hpp"

TEST(relaxed_constexpr, factorial_zero_is_one)
{
    /* The base case the stub omits. */
    EXPECT_EQ(task01::factorial(0), 1);
}

TEST(relaxed_constexpr, factorial_one_is_one)
{
    EXPECT_EQ(task01::factorial(1), 1);
}

TEST(relaxed_constexpr, factorial_two_is_two)
{
    EXPECT_EQ(task01::factorial(2), 2);
}

TEST(relaxed_constexpr, factorial_five_is_120)
{
    EXPECT_EQ(task01::factorial(5), 120);
}

TEST(relaxed_constexpr, factorial_ten_is_3628800)
{
    EXPECT_EQ(task01::factorial(10), 3628800);
}

TEST(relaxed_constexpr, factorial_twelve_fits_in_long_long)
{
    EXPECT_EQ(task01::factorial(12), 479001600LL);
}

TEST(relaxed_constexpr, evaluable_at_compile_time)
{
    /* The whole POINT of constexpr: usable at compile time. The
     * `constexpr` variable declaration itself enforces compile-
     * time evaluation -- if `factorial(5)` isn't constexpr-ready,
     * this line won't compile. */
    constexpr long long f5 = task01::factorial(5);
    EXPECT_EQ(f5, 120);
}

TEST(relaxed_constexpr, compile_time_zero_case)
{
    constexpr long long f0 = task01::factorial(0);
    EXPECT_EQ(f0, 1);
}

TEST(relaxed_constexpr, compile_time_ten)
{
    constexpr long long f10 = task01::factorial(10);
    EXPECT_EQ(f10, 3628800);
}
