#include <gtest/gtest.h>

#include <map>
#include <string>
#include <tuple>
#include <utility>

#include "solution.hpp"

namespace t = task02;

TEST(structured_bindings_tuples, insert_new_key_returns_true)
{
    std::map<int, int> m;
    EXPECT_TRUE(t::try_insert(m, 1, 100));
    EXPECT_EQ(m[1], 100);
}

TEST(structured_bindings_tuples, insert_existing_key_returns_false)
{
    std::map<int, int> m;
    t::try_insert(m, 1, 100);
    EXPECT_FALSE(t::try_insert(m, 1, 200));
    EXPECT_EQ(m[1], 100); /* original value preserved */
}

TEST(structured_bindings_tuples, multiple_distinct_insertions)
{
    std::map<int, int> m;
    EXPECT_TRUE(t::try_insert(m, 1, 10));
    EXPECT_TRUE(t::try_insert(m, 2, 20));
    EXPECT_TRUE(t::try_insert(m, 3, 30));
    EXPECT_EQ(m.size(), 3u);
}

TEST(structured_bindings_tuples, pair_sum_simple)
{
    EXPECT_EQ(t::pair_sum(std::make_pair(1, 2)), 3);
}

TEST(structured_bindings_tuples, pair_sum_negatives)
{
    EXPECT_EQ(t::pair_sum(std::make_pair(-10, 5)), -5);
}

TEST(structured_bindings_tuples, pair_sum_zero)
{
    /* Edge case: matches stub. Sanity test. */
    EXPECT_EQ(t::pair_sum(std::make_pair(0, 0)), 0);
}

TEST(structured_bindings_tuples, tuple_summary_alice)
{
    auto s = t::tuple_summary(std::make_tuple(30, std::string("alice"), 3.14));
    EXPECT_EQ(s, "alice:30");
}

TEST(structured_bindings_tuples, tuple_summary_bob)
{
    auto s = t::tuple_summary(std::make_tuple(42, std::string("bob"), 1.0));
    EXPECT_EQ(s, "bob:42");
}

TEST(structured_bindings_tuples, tuple_score_value)
{
    EXPECT_DOUBLE_EQ(t::tuple_score(std::make_tuple(1, std::string("x"), 7.25)),
                     7.25);
}

TEST(structured_bindings_tuples, direct_pair_binding)
{
    auto [a, b] = std::make_pair(100, 200);
    EXPECT_EQ(a, 100);
    EXPECT_EQ(b, 200);
}

TEST(structured_bindings_tuples, direct_tuple_binding)
{
    auto [x, y, z] = std::make_tuple(1, 2.5, std::string("hi"));
    EXPECT_EQ(x, 1);
    EXPECT_DOUBLE_EQ(y, 2.5);
    EXPECT_EQ(z, "hi");
}

TEST(structured_bindings_tuples, range_for_with_structured_binding)
{
    /* The most ergonomic combination: range-for + structured
     * binding to walk a map. */
    std::map<int, std::string> m = {{1, "a"}, {2, "b"}, {3, "c"}};
    std::string keys;
    std::string vals;
    for (auto const &[k, v] : m)
    {
        keys += std::to_string(k);
        vals += v;
    }
    EXPECT_EQ(keys, "123");
    EXPECT_EQ(vals, "abc");
}
