#include <gtest/gtest.h>

#include <tuple>

#include "solution.hpp"

namespace t = task01;

TEST(structured_bindings_arrays, destructure_one_two_three)
{
    int arr[3] = {1, 2, 3};
    auto [a, b, c] = t::destructure(arr);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
}

TEST(structured_bindings_arrays, destructure_negatives)
{
    int arr[3] = {-10, -20, -30};
    auto [a, b, c] = t::destructure(arr);
    EXPECT_EQ(a, -10);
    EXPECT_EQ(b, -20);
    EXPECT_EQ(c, -30);
}

TEST(structured_bindings_arrays, destructure_zero_array)
{
    /* Edge case: all zeros. Matches the stub's hardcoded
     * value -- sanity test only. */
    int arr[3] = {0, 0, 0};
    auto [a, b, c] = t::destructure(arr);
    EXPECT_EQ(a, 0);
    EXPECT_EQ(b, 0);
    EXPECT_EQ(c, 0);
}

TEST(structured_bindings_arrays, destructure5_five_values)
{
    int arr[5] = {10, 20, 30, 40, 50};
    auto [a, b, c, d, e] = t::destructure5(arr);
    EXPECT_EQ(a, 10);
    EXPECT_EQ(b, 20);
    EXPECT_EQ(c, 30);
    EXPECT_EQ(d, 40);
    EXPECT_EQ(e, 50);
}

TEST(structured_bindings_arrays, destructure5_sequential)
{
    int arr[5] = {1, 2, 3, 4, 5};
    auto [a, b, c, d, e] = t::destructure5(arr);
    EXPECT_EQ(a + b + c + d + e, 15);
}

TEST(structured_bindings_arrays, sum3_simple)
{
    int arr[3] = {1, 2, 3};
    EXPECT_EQ(t::sum3(arr), 6);
}

TEST(structured_bindings_arrays, sum3_large_values)
{
    int arr[3] = {100, 200, 300};
    EXPECT_EQ(t::sum3(arr), 600);
}

TEST(structured_bindings_arrays, sum3_negatives_cancel)
{
    int arr[3] = {10, -5, -5};
    EXPECT_EQ(t::sum3(arr), 0);
}

TEST(structured_bindings_arrays, direct_array_binding)
{
    /* The textbook usage: bind the array directly at the
     * call site. */
    int arr[3] = {7, 8, 9};
    auto [a, b, c] = arr;
    EXPECT_EQ(a, 7);
    EXPECT_EQ(b, 8);
    EXPECT_EQ(c, 9);
}

TEST(structured_bindings_arrays, binding_reference_aliases)
{
    /* `auto& [a, b, c] = arr;` binds a, b, c as references
     * to the array elements -- writes through them update
     * the array. */
    int arr[3] = {0, 0, 0};
    auto &[a, b, c] = arr;
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}
