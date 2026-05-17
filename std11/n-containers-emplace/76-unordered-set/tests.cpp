#include <gtest/gtest.h>

#include <unordered_set>
#include <vector>

#include "solution.hpp"

TEST(unordered_set, dedup_empty)
{
    EXPECT_EQ(task76::dedup({}), std::vector<int>{});
}

TEST(unordered_set, dedup_all_unique_preserves_order)
{
    EXPECT_EQ(task76::dedup({1, 2, 3, 4, 5}), (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(unordered_set, dedup_all_same)
{
    EXPECT_EQ(task76::dedup({7, 7, 7, 7}), std::vector<int>{7});
}

TEST(unordered_set, dedup_mixed_first_seen_order)
{
    /* Output is the first-seen-order projection: 3, 1, 4, 1, 5, 9, 2, 6
     * with later 1 dropped -> 3, 1, 4, 5, 9, 2, 6. */
    EXPECT_EQ(task76::dedup({3, 1, 4, 1, 5, 9, 2, 6}), (std::vector<int>{3, 1, 4, 5, 9, 2, 6}));
}

TEST(unordered_set, dedup_negatives_and_zero)
{
    EXPECT_EQ(task76::dedup({0, -1, 2, -1, 0, 3}), (std::vector<int>{0, -1, 2, 3}));
}

TEST(unordered_set, dedup_large_input)
{
    std::vector<int> in;
    in.reserve(2000);
    for (int i = 0; i < 1000; ++i)
    {
        in.push_back(i);
        in.push_back(i); /* every element twice */
    }
    auto out = task76::dedup(in);
    ASSERT_EQ(out.size(), 1000u);
    for (int i = 0; i < 1000; ++i)
    {
        EXPECT_EQ(out[static_cast<std::size_t>(i)], i) << "i=" << i;
    }
}

TEST(unordered_set, set_of_empty)
{
    auto s = task76::set_of({});
    EXPECT_TRUE(s.empty());
}

TEST(unordered_set, set_of_collapses_duplicates)
{
    auto s = task76::set_of({3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5});
    EXPECT_EQ(s.size(), 7u);
    for (int v : {1, 2, 3, 4, 5, 6, 9})
    {
        EXPECT_EQ(s.count(v), 1u) << "missing " << v;
    }
    EXPECT_EQ(s.count(0), 0u);
    EXPECT_EQ(s.count(100), 0u);
}

TEST(unordered_set, set_of_equals_unordered_set_constructed_from_dedup)
{
    /* dedup followed by `unordered_set::range` should give the same
     * set as `set_of` -- the contents are the unique elements. */
    std::vector<int> in{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    auto s_direct = task76::set_of(in);
    auto deduped = task76::dedup(in);
    std::unordered_set<int> s_from_dedup(deduped.begin(), deduped.end());
    EXPECT_EQ(s_direct, s_from_dedup);
}
