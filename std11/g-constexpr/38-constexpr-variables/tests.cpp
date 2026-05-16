#include <gtest/gtest.h>

#include <array>
#include <cstddef>

#include "solution.hpp"

/* Compile-time checks: each constant is usable in static_assert. */
static_assert(task38::k_size == 16, "k_size must be 16");
static_assert(task38::k_buffer_capacity == 1024u, "k_buffer_capacity must be 1024");
static_assert(task38::k_max_retries == 3, "k_max_retries must be 3");

/* k_pi is a double: floating-point equality at compile time. */
static_assert(task38::k_pi > 3.14 && task38::k_pi < 3.15, "k_pi must be near 3.14159...");

/* k_size works as an array bound at namespace scope. */
namespace
{
int reference_array[task38::k_size];
}
static_assert(sizeof(reference_array) == task38::k_size * sizeof(int), "array bound uses k_size");

/* k_buffer_capacity works as a std::array<T, N> non-type template arg. */
static_assert(std::tuple_size<std::array<int, task38::k_buffer_capacity>>::value == task38::k_buffer_capacity, "k_buffer_capacity works as template arg");

TEST(constexpr_variables, k_size_runtime_value)
{
    EXPECT_EQ(task38::k_size, 16);
}

TEST(constexpr_variables, k_buffer_capacity_runtime_value)
{
    EXPECT_EQ(task38::k_buffer_capacity, 1024u);
}

TEST(constexpr_variables, k_pi_runtime_value)
{
    EXPECT_GT(task38::k_pi, 3.14);
    EXPECT_LT(task38::k_pi, 3.15);
}

TEST(constexpr_variables, k_max_retries_runtime_value)
{
    EXPECT_EQ(task38::k_max_retries, 3);
}

TEST(constexpr_variables, get_buffer_returns_non_null)
{
    EXPECT_NE(task38::get_buffer(), nullptr);
}

TEST(constexpr_variables, get_buffer_is_zero_initialized)
{
    const int *p = task38::get_buffer();
    ASSERT_NE(p, nullptr);
    for (int i = 0; i < task38::k_size; ++i)
    {
        EXPECT_EQ(p[i], 0) << "index " << i;
    }
}

TEST(constexpr_variables, get_buffer_returns_stable_address)
{
    /* A function-local static has a single address across calls. */
    EXPECT_EQ(task38::get_buffer(), task38::get_buffer());
}
