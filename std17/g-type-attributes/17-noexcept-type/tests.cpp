#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

namespace t = task17;

TEST(noexcept_type, throw_fp_call_result)
{
    EXPECT_EQ(t::call_through_throw_fp(5), 25);
    EXPECT_EQ(t::call_through_throw_fp(7), 49);
}

TEST(noexcept_type, noexcept_fp_call_result)
{
    EXPECT_EQ(t::call_through_noexcept_fp(4), 16);
    EXPECT_EQ(t::call_through_noexcept_fp(0), 0);
}

TEST(noexcept_type, fp_types_differ)
{
    /* In C++17, `int(*)(int)` and `int(*)(int) noexcept` are
     * DIFFERENT types. */
    using fp_throw = int (*)(int);
    using fp_noexcept = int (*)(int) noexcept;
    EXPECT_FALSE((std::is_same<fp_throw, fp_noexcept>::value));
}

TEST(noexcept_type, noexcept_fp_convertible_to_throw_fp)
{
    /* You can weaken the guarantee: noexcept fp -> throwing
     * fp is legal. */
    using fp_throw = int (*)(int);
    using fp_noexcept = int (*)(int) noexcept;
    EXPECT_TRUE((std::is_convertible<fp_noexcept, fp_throw>::value));
}

TEST(noexcept_type, throw_fp_NOT_convertible_to_noexcept_fp)
{
    /* You can't strengthen: throwing fp -> noexcept fp is
     * NOT a valid implicit conversion. */
    using fp_throw = int (*)(int);
    using fp_noexcept = int (*)(int) noexcept;
    EXPECT_FALSE((std::is_convertible<fp_throw, fp_noexcept>::value));
}
