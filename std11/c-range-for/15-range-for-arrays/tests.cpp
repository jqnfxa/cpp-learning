#include <gtest/gtest.h>

#include "solution.hpp"

TEST(range_for_arrays, sum_small_positive)
{
    const int arr[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(task15::sum(arr), 15);
}

TEST(range_for_arrays, sum_single_element)
{
    const int arr[] = {0};
    EXPECT_EQ(task15::sum(arr), 0);
    const int one[] = {42};
    EXPECT_EQ(task15::sum(one), 42);
}

TEST(range_for_arrays, sum_with_negatives)
{
    const int arr[] = {-3, 5, -2, 4};
    EXPECT_EQ(task15::sum(arr), 4);
}

TEST(range_for_arrays, sum_large_array)
{
    int arr[100];
    for (int i = 0; i < 100; ++i)
    {
        arr[i] = i + 1;
    }
    /* 1 + 2 + ... + 100 = 5050. */
    EXPECT_EQ(task15::sum(arr), 5050);
}

TEST(range_for_arrays, count_evens_mixed)
{
    const int arr[] = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(task15::count_evens(arr), 3u);
}

TEST(range_for_arrays, count_evens_all_odd)
{
    const int arr[] = {1, 3, 5, 7};
    EXPECT_EQ(task15::count_evens(arr), 0u);
}

TEST(range_for_arrays, count_evens_all_even)
{
    const int arr[] = {2, 4, 6, 8};
    EXPECT_EQ(task15::count_evens(arr), 4u);
}

TEST(range_for_arrays, count_evens_zero_counts)
{
    /* 0 is even -- it has no remainder when divided by 2. */
    const int arr[] = {0, 1, 0, 1};
    EXPECT_EQ(task15::count_evens(arr), 2u);
}
