#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

namespace t = task130;

TEST(scoped_allocator, log_size_is_three)
{
    /* Outer reserve(2) + two inner push_back == 3 allocations. */
    auto tags = t::matrix_alloc_tags(7);
    EXPECT_EQ(tags.size(), 3u);
}

TEST(scoped_allocator, outer_allocation_uses_supplied_tag)
{
    /* This is the OUTER vector's reserve(2). Even the
     * non-scoped scaffold gets this one right -- the outer's own
     * allocator is the one we passed in. */
    auto tags = t::matrix_alloc_tags(7);
    ASSERT_GE(tags.size(), 1u);
    EXPECT_EQ(tags[0], 7);
}

TEST(scoped_allocator, inner_allocations_inherit_outer_tag)
{
    /* The KEY test: with scoped_allocator_adaptor, the inner
     * push_back must report the same tag as the outer. Without
     * the adaptor, the inner vector has a default-constructed
     * allocator (tag=0) and these tags will be 0. */
    auto tags = t::matrix_alloc_tags(7);
    ASSERT_GE(tags.size(), 3u);
    EXPECT_EQ(tags[1], 7);
    EXPECT_EQ(tags[2], 7);
}

TEST(scoped_allocator, propagation_works_for_different_tags)
{
    auto tags42 = t::matrix_alloc_tags(42);
    ASSERT_EQ(tags42.size(), 3u);
    EXPECT_EQ(tags42[0], 42);
    EXPECT_EQ(tags42[1], 42);
    EXPECT_EQ(tags42[2], 42);
}

TEST(scoped_allocator, all_tags_uniform)
{
    auto tags = t::matrix_alloc_tags(99);
    ASSERT_FALSE(tags.empty());
    for (int x : tags)
    {
        EXPECT_EQ(x, 99);
    }
}

TEST(scoped_allocator, distinct_calls_dont_bleed)
{
    /* reset_log() at the start of each call must clear the log
     * so the second call doesn't accumulate. */
    t::matrix_alloc_tags(1);
    auto tags = t::matrix_alloc_tags(2);
    EXPECT_EQ(tags.size(), 3u);
    EXPECT_EQ(tags[0], 2);
}

TEST(scoped_allocator, zero_tag_distinguishable)
{
    /* Sanity check: tag 0 is itself a legal tag and the outer
     * still reports it for the outer allocation. The "is 0 a
     * special value" worry: nope, it's just a number. */
    auto tags = t::matrix_alloc_tags(0);
    ASSERT_GE(tags.size(), 1u);
    EXPECT_EQ(tags[0], 0);
}

TEST(scoped_allocator, tagged_allocator_equality_uses_tag)
{
    t::tagged_alloc<int> a3(3);
    t::tagged_alloc<int> b3(3);
    t::tagged_alloc<int> c4(4);
    EXPECT_TRUE(a3 == b3);
    EXPECT_FALSE(a3 == c4);
}
