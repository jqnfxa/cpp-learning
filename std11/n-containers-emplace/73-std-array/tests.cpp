#include <gtest/gtest.h>

#include <array>

#include "solution.hpp"

/* sizeof: std::array is exactly N * sizeof(T) -- no header, no extras. */
static_assert(sizeof(std::array<double, 4>) == 4 * sizeof(double),
              "std::array<T, N> must be tight (N * sizeof(T))");

TEST(std_array, sum_simple)
{
    std::array<double, 4> a{1.0, 2.0, 3.0, 4.0};
    EXPECT_DOUBLE_EQ(task73::sum(a), 10.0);
}

TEST(std_array, sum_zeros)
{
    std::array<double, 4> a{};
    EXPECT_DOUBLE_EQ(task73::sum(a), 0.0);
}

TEST(std_array, sum_negatives)
{
    std::array<double, 4> a{-1.5, -2.5, 0.0, 4.0};
    EXPECT_DOUBLE_EQ(task73::sum(a), 0.0);
}

TEST(std_array, mean_simple)
{
    std::array<double, 4> a{1.0, 2.0, 3.0, 4.0};
    EXPECT_DOUBLE_EQ(task73::mean(a), 2.5);
}

TEST(std_array, mean_uniform)
{
    std::array<double, 4> a{7.0, 7.0, 7.0, 7.0};
    EXPECT_DOUBLE_EQ(task73::mean(a), 7.0);
}

TEST(std_array, add_componentwise)
{
    std::array<double, 4> x{1.0, 2.0, 3.0, 4.0};
    std::array<double, 4> y{10.0, 20.0, 30.0, 40.0};

    auto z = task73::add(x, y);
    EXPECT_DOUBLE_EQ(z[0], 11.0);
    EXPECT_DOUBLE_EQ(z[1], 22.0);
    EXPECT_DOUBLE_EQ(z[2], 33.0);
    EXPECT_DOUBLE_EQ(z[3], 44.0);
}

TEST(std_array, add_zero_identity)
{
    std::array<double, 4> x{1.5, 2.5, 3.5, 4.5};
    std::array<double, 4> z{};
    EXPECT_EQ(task73::add(x, z), x);
    EXPECT_EQ(task73::add(z, x), x);
}

TEST(std_array, add_returns_value_not_reference)
{
    /* add() returns by value; the inputs must be unchanged afterwards. */
    std::array<double, 4> x{1, 2, 3, 4};
    std::array<double, 4> y{4, 3, 2, 1};
    std::array<double, 4> x_copy = x;
    std::array<double, 4> y_copy = y;

    (void)task73::add(x, y);
    EXPECT_EQ(x, x_copy);
    EXPECT_EQ(y, y_copy);
}
