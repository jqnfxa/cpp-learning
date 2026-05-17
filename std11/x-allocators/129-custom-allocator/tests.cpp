#include <gtest/gtest.h>

#include <cstddef>
#include <type_traits>
#include <vector>

#include "solution.hpp"

namespace t = task129;

class CountingAllocatorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        t::reset_stats();
    }
};

TEST_F(CountingAllocatorTest, has_value_type)
{
    static_assert(
        std::is_same<t::counting_allocator<int>::value_type, int>::value,
        "value_type alias must be T");
    SUCCEED();
}

TEST_F(CountingAllocatorTest, alloc_then_dealloc_bumps_both_counters)
{
    t::counting_allocator<int> a;
    int *p = a.allocate(4);
    EXPECT_EQ(t::stats().allocations, 1u);
    EXPECT_EQ(t::stats().bytes_allocated, 4u * sizeof(int));
    a.deallocate(p, 4);
    EXPECT_EQ(t::stats().deallocations, 1u);
}

TEST_F(CountingAllocatorTest, vector_constructor_does_not_allocate_when_empty)
{
    t::ivec v;
    EXPECT_EQ(t::stats().allocations, 0u);
}

TEST_F(CountingAllocatorTest, vector_push_back_triggers_allocate)
{
    t::ivec v;
    v.push_back(1);
    EXPECT_GE(t::stats().allocations, 1u);
}

TEST_F(CountingAllocatorTest, vector_growth_triggers_multiple_allocates)
{
    t::ivec v;
    for (int i = 0; i < 100; ++i)
    {
        v.push_back(i);
    }
    /* Geometric growth -> several reallocations; at least 2. */
    EXPECT_GE(t::stats().allocations, 2u);
}

TEST_F(CountingAllocatorTest, vector_destruction_calls_deallocate)
{
    {
        t::ivec v;
        v.reserve(8);
        EXPECT_GE(t::stats().allocations, 1u);
        EXPECT_EQ(t::stats().deallocations, 0u);
    }
    EXPECT_GE(t::stats().deallocations, 1u);
}

TEST_F(CountingAllocatorTest, allocate_then_deallocate_balance)
{
    {
        t::ivec v;
        for (int i = 0; i < 50; ++i)
        {
            v.push_back(i);
        }
    }
    EXPECT_EQ(t::stats().allocations, t::stats().deallocations);
}

TEST_F(CountingAllocatorTest, reserve_records_bytes)
{
    t::ivec v;
    v.reserve(10);
    EXPECT_GE(t::stats().bytes_allocated, 10u * sizeof(int));
}

TEST_F(CountingAllocatorTest, vector_stores_values_correctly)
{
    t::ivec v;
    for (int i = 0; i < 5; ++i)
    {
        v.push_back(i * 7);
    }
    ASSERT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[4], 28);
}

TEST_F(CountingAllocatorTest, rebind_allocator_compares_equal)
{
    /* Stateless allocators of related types must compare equal:
     * std::vector uses this to know it can move-construct from
     * one allocator to another. */
    t::counting_allocator<int> ai;
    t::counting_allocator<double> ad;
    EXPECT_TRUE(ai == ad);
    EXPECT_FALSE(ai != ad);
}

TEST_F(CountingAllocatorTest, multiple_vectors_share_stats)
{
    {
        t::ivec a;
        a.push_back(1);
        t::ivec b;
        b.push_back(2);
    }
    /* Both vectors should have contributed to the global counters. */
    EXPECT_GE(t::stats().allocations, 2u);
    EXPECT_GE(t::stats().deallocations, 2u);
}
