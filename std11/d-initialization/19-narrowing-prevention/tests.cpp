#include <gtest/gtest.h>

#include "solution.hpp"

/*
 * The compile-time block pins the narrowing rules with the SFINAE
 * trait. Each `is_brace_constructible_from<To, From>::value`
 * answers: "would `To{from}` compile?"
 */

/* Identity is always allowed. */
static_assert(task19::is_brace_constructible_from<int, int>::value, "int{int} -- identity");
static_assert(task19::is_brace_constructible_from<double, double>::value, "double{double} -- identity");

/* Pure integer widening (no information loss possible) is allowed. */
static_assert(task19::is_brace_constructible_from<int, char>::value, "int{char} -- widening");
static_assert(task19::is_brace_constructible_from<int, short>::value, "int{short} -- widening");
static_assert(task19::is_brace_constructible_from<long long, int>::value, "long long{int} -- widening");

/* Narrowing pairs -- rejected. */
static_assert(!task19::is_brace_constructible_from<int, double>::value, "int{double} -- float to int is narrowing");
static_assert(!task19::is_brace_constructible_from<int, float>::value, "int{float} -- float to int is narrowing");
static_assert(!task19::is_brace_constructible_from<short, int>::value, "short{int} -- wider int to narrower int is narrowing");
static_assert(!task19::is_brace_constructible_from<float, double>::value, "float{double} -- wider FP to narrower FP is narrowing");

TEST(narrowing_prevention, truncate_to_int_drops_fraction_positive)
{
    EXPECT_EQ(task19::truncate_to_int(3.5), 3);
    EXPECT_EQ(task19::truncate_to_int(0.9), 0);
    EXPECT_EQ(task19::truncate_to_int(42.999), 42);
}

TEST(narrowing_prevention, truncate_to_int_drops_fraction_negative)
{
    /* Truncation toward zero: -2.7 -> -2, not -3. */
    EXPECT_EQ(task19::truncate_to_int(-2.7), -2);
    EXPECT_EQ(task19::truncate_to_int(-0.1), 0);
}

TEST(narrowing_prevention, truncate_to_int_exact)
{
    EXPECT_EQ(task19::truncate_to_int(0.0), 0);
    EXPECT_EQ(task19::truncate_to_int(7.0), 7);
}

TEST(narrowing_prevention, truncate_to_int_returns_int)
{
    /* truncate_to_int returns int, not double. The static_assert
     * pins this. */
    using R = decltype(task19::truncate_to_int(0.0));
    static_assert(std::is_same<R, int>::value, "truncate_to_int must return int");
    SUCCEED();
}
