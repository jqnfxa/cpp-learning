#include <gtest/gtest.h>

#include "solution.hpp"

TEST(capture_by_reference, count_up_to_zero)
{
    EXPECT_EQ(task50::count_up_to(0), 0);
}

TEST(capture_by_reference, count_up_to_one)
{
    EXPECT_EQ(task50::count_up_to(1), 1);
}

TEST(capture_by_reference, count_up_to_five)
{
    EXPECT_EQ(task50::count_up_to(5), 5);
}

TEST(capture_by_reference, count_up_to_hundred)
{
    EXPECT_EQ(task50::count_up_to(100), 100);
}

TEST(capture_by_reference, sum_empty)
{
    EXPECT_EQ(task50::sum_via_reference_capture(nullptr, 0), 0);
}

TEST(capture_by_reference, sum_single_element)
{
    int arr[] = {42};
    EXPECT_EQ(task50::sum_via_reference_capture(arr, 1), 42);
}

TEST(capture_by_reference, sum_five_elements)
{
    int arr[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(task50::sum_via_reference_capture(arr, 5), 15);
}

TEST(capture_by_reference, sum_with_negatives)
{
    int arr[] = {-3, 5, -2, 4};
    EXPECT_EQ(task50::sum_via_reference_capture(arr, 4), 4);
}

TEST(capture_by_reference, sum_large_array)
{
    int arr[100];
    for (int i = 0; i < 100; ++i)
    {
        arr[i] = i + 1;
    }
    EXPECT_EQ(task50::sum_via_reference_capture(arr, 100), 5050);
}
