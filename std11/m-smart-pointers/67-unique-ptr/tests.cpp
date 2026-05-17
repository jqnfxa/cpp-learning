#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <utility>

#include "solution.hpp"

TEST(unique_ptr_int_array, make_zeros_yields_owning_pointer)
{
    auto arr = task67::make_zeros(8);
    ASSERT_NE(arr.get(), nullptr);
    for (std::size_t i = 0; i < 8; ++i)
    {
        EXPECT_EQ(arr[i], 0) << "i=" << i;
    }
}

TEST(unique_ptr_int_array, make_zeros_size_zero_is_allowed)
{
    /* `new int[0]()` is valid and yields a non-null pointer; the
     * unique_ptr<int[]> must still call delete[] on it.  We only
     * exercise the alloc/dealloc path -- nothing to read. */
    auto arr = task67::make_zeros(0);
    SUCCEED();
    (void)arr;
}

TEST(unique_ptr_int_array, sum_simple)
{
    auto arr = task67::make_zeros(5);
    ASSERT_NE(arr.get(), nullptr); /* guard: indexing a null unique_ptr is UB. */
    for (int i = 0; i < 5; ++i)
    {
        arr[static_cast<std::size_t>(i)] = i + 1; /* 1 2 3 4 5 */
    }
    EXPECT_EQ(task67::sum(arr, 5), 15);
}

TEST(unique_ptr_int_array, sum_empty)
{
    auto arr = task67::make_zeros(0);
    EXPECT_EQ(task67::sum(arr, 0), 0);
}

TEST(unique_ptr_int_array, move_transfers_ownership)
{
    auto a = task67::make_zeros(4);
    ASSERT_NE(a.get(), nullptr);
    int *raw = a.get();
    auto b = std::move(a);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(b.get(), raw);
}

TEST(unique_ptr_int_array, reset_releases)
{
    auto a = task67::make_zeros(4);
    ASSERT_NE(a.get(), nullptr);
    a.reset();
    EXPECT_EQ(a.get(), nullptr);
}

TEST(unique_ptr_int_array, safe_average_normal)
{
    /* Reference impl fills with zeros, so sum is 0 and avg is 0/2 = 0. */
    EXPECT_EQ(task67::safe_average(10, 2), 0);
}

TEST(unique_ptr_int_array, safe_average_throws_on_zero_denominator)
{
    EXPECT_THROW(task67::safe_average(10, 0), std::runtime_error);
}

TEST(unique_ptr_int_array, safe_average_loop_does_not_leak)
{
    /* If safe_average leaked its local buffer on the throw path, a tight
     * loop would push virtual memory hard.  Without a sanitiser we can't
     * directly detect a leak, but we can observe that the function
     * remains responsive across many iterations: the exception
     * propagates, and another call still succeeds.  Under
     * ASAN/LSAN this loop also fails on actual leaks. */
    for (int i = 0; i < 4096; ++i)
    {
        EXPECT_THROW(task67::safe_average(64, 0), std::runtime_error);
    }
    EXPECT_EQ(task67::safe_average(64, 4), 0);
}
