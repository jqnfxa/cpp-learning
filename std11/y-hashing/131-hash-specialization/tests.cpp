#include <gtest/gtest.h>

#include <cstddef>
#include <functional>
#include <unordered_set>

#include "solution.hpp"

namespace t = task131;

TEST(hash_specialization, equal_points_compare_equal)
{
    EXPECT_TRUE((t::point{1, 2}) == (t::point{1, 2}));
}

TEST(hash_specialization, different_points_compare_unequal)
{
    EXPECT_FALSE((t::point{1, 2}) == (t::point{2, 1}));
}

TEST(hash_specialization, equal_points_hash_equal)
{
    /* This is the hard contract for std::hash:
     * a == b implies hash(a) == hash(b). */
    std::hash<t::point> h;
    EXPECT_EQ(h(t::point{3, 4}), h(t::point{3, 4}));
}

TEST(hash_specialization, set_inserts_distinct_points)
{
    t::point_set s;
    s.insert(t::point{1, 2});
    s.insert(t::point{2, 1});
    s.insert(t::point{0, 0});
    EXPECT_EQ(s.size(), 3u);
}

TEST(hash_specialization, set_dedups_equal_points)
{
    t::point_set s;
    s.insert(t::point{5, 5});
    s.insert(t::point{5, 5});
    s.insert(t::point{5, 5});
    EXPECT_EQ(s.size(), 1u);
}

TEST(hash_specialization, set_find_finds)
{
    t::point_set s = {{1, 2}, {3, 4}, {5, 6}};
    EXPECT_NE(s.find(t::point{3, 4}), s.end());
    EXPECT_EQ(s.find(t::point{9, 9}), s.end());
}

TEST(hash_specialization, hash_distinguishes_xy_swap)
{
    /* The classic trap: a naive `return x + y` would hash
     * {1, 2} and {2, 1} to the same bucket. They're distinct
     * points -- a good hash should usually separate them.
     *
     * "Usually" because hashes CAN collide; but the trivial
     * stub returning 0 for everything always collides, and
     * even `x + y` always collides for any swap pair. A
     * proper hash_combine yields different values. */
    std::hash<t::point> h;
    EXPECT_NE(h(t::point{1, 2}), h(t::point{2, 1}));
}

TEST(hash_specialization, hash_distinguishes_origin_from_nearby)
{
    std::hash<t::point> h;
    EXPECT_NE(h(t::point{0, 0}), h(t::point{0, 1}));
    EXPECT_NE(h(t::point{0, 0}), h(t::point{1, 0}));
}

TEST(hash_specialization, hash_not_constant_across_many_points)
{
    /* Generate 100 distinct points and require at least 50
     * distinct hash values. This is conservative -- a good
     * hash would give ~100; the all-zero stub gives exactly 1. */
    std::hash<t::point> h;
    std::unordered_set<std::size_t> seen;
    for (int i = 0; i < 100; ++i)
    {
        seen.insert(h(t::point{i, i * 3 + 1}));
    }
    EXPECT_GE(seen.size(), 50u);
}

TEST(hash_specialization, set_handles_many_inserts_fast)
{
    /* Sanity: insert 1000 distinct points, lookup each. With
     * the all-zero hash, this still works correctly (the
     * equality comparator handles dedup) but degrades to O(n^2)
     * -- which is still fast enough at n=1000 to pass.
     *
     * The point: this test doesn't pin performance, just
     * functional correctness. */
    t::point_set s;
    for (int i = 0; i < 1000; ++i)
    {
        s.insert(t::point{i, i * 2});
    }
    EXPECT_EQ(s.size(), 1000u);
    for (int i = 0; i < 1000; ++i)
    {
        EXPECT_NE(s.find(t::point{i, i * 2}), s.end());
    }
}

TEST(hash_specialization, erase_removes_one)
{
    t::point_set s = {{1, 1}, {2, 2}, {3, 3}};
    EXPECT_EQ(s.erase(t::point{2, 2}), 1u);
    EXPECT_EQ(s.size(), 2u);
    EXPECT_EQ(s.find(t::point{2, 2}), s.end());
}
