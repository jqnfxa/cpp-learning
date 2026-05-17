#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "solution.hpp"

TEST(unordered_map, word_count_empty)
{
    auto m = task75::word_count({});
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

TEST(unordered_map, word_count_single_word)
{
    auto m = task75::word_count({"alpha"});
    ASSERT_EQ(m.size(), 1u);
    EXPECT_EQ(m["alpha"], 1u);
}

TEST(unordered_map, word_count_distinct_words)
{
    auto m = task75::word_count({"a", "b", "c"});
    EXPECT_EQ(m.size(), 3u);
    EXPECT_EQ(m["a"], 1u);
    EXPECT_EQ(m["b"], 1u);
    EXPECT_EQ(m["c"], 1u);
}

TEST(unordered_map, word_count_repeats)
{
    auto m = task75::word_count({"apple", "banana", "apple", "cherry", "apple", "banana"});
    EXPECT_EQ(m.size(), 3u);
    EXPECT_EQ(m["apple"], 3u);
    EXPECT_EQ(m["banana"], 2u);
    EXPECT_EQ(m["cherry"], 1u);
}

TEST(unordered_map, word_count_matches_expected_map)
{
    auto m = task75::word_count({"x", "y", "x", "z", "y", "x"});
    task75::counts expected{{"x", 3}, {"y", 2}, {"z", 1}};
    EXPECT_EQ(m, expected);
}

TEST(unordered_map, count_of_present)
{
    task75::counts m{{"alpha", 3}, {"beta", 5}};
    EXPECT_EQ(task75::count_of(m, "alpha"), 3u);
    EXPECT_EQ(task75::count_of(m, "beta"), 5u);
}

TEST(unordered_map, count_of_missing_returns_zero)
{
    task75::counts m{{"alpha", 3}};
    EXPECT_EQ(task75::count_of(m, "missing"), 0u);
}

TEST(unordered_map, count_of_does_not_mutate)
{
    /* The signature passes `m` as const ref, so anything that would
     * mutate (e.g. operator[]) would fail to compile. This test
     * verifies the map is unchanged after a missing-key lookup
     * by checking size and contents before/after. */
    task75::counts m{{"alpha", 1}};
    const auto before_size = m.size();
    (void)task75::count_of(m, "missing");
    EXPECT_EQ(m.size(), before_size);
    EXPECT_EQ(m.count("missing"), 0u);
}

TEST(unordered_map, word_count_handles_long_input)
{
    std::vector<std::string> input;
    input.reserve(1000);
    for (int i = 0; i < 1000; ++i)
    {
        input.push_back((i % 3 == 0) ? "fizz" : ((i % 5 == 0) ? "buzz" : "other"));
    }
    auto m = task75::word_count(input);
    /* 0..999: fizz when i%3==0 -> 334 (0,3,6,...,999). buzz when
     * i%5==0 AND i%3!=0 -> i in {5,10,20,25,...}. other -> the rest. */
    std::size_t fizz = 0, buzz = 0, other = 0;
    for (int i = 0; i < 1000; ++i)
    {
        if (i % 3 == 0)
            ++fizz;
        else if (i % 5 == 0)
            ++buzz;
        else
            ++other;
    }
    EXPECT_EQ(m["fizz"], fizz);
    EXPECT_EQ(m["buzz"], buzz);
    EXPECT_EQ(m["other"], other);
}
