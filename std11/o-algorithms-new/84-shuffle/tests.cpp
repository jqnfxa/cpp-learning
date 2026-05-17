#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <set>
#include <vector>

#include "solution.hpp"

TEST(shuffle, make_deck_is_ordered_zero_to_fifty_one)
{
    auto d = task84::make_deck();
    ASSERT_EQ(d.size(), 52u);
    for (int i = 0; i < 52; ++i)
    {
        EXPECT_EQ(d[static_cast<std::size_t>(i)], i);
    }
}

TEST(shuffle, shuffle_preserves_permutation_property)
{
    auto d = task84::make_deck();
    std::mt19937 rng(2026);
    task84::shuffle_deck(d, rng);

    ASSERT_EQ(d.size(), 52u);
    std::sort(d.begin(), d.end());
    for (int i = 0; i < 52; ++i)
    {
        EXPECT_EQ(d[static_cast<std::size_t>(i)], i) << "missing or duplicated at i=" << i;
    }
}

TEST(shuffle, same_seed_yields_same_shuffle)
{
    auto a = task84::make_deck();
    auto b = task84::make_deck();

    std::mt19937 rng1(42);
    std::mt19937 rng2(42);
    task84::shuffle_deck(a, rng1);
    task84::shuffle_deck(b, rng2);

    EXPECT_EQ(a, b) << "deterministic given the same seed";
}

TEST(shuffle, different_seeds_typically_yield_different_shuffles)
{
    auto a = task84::make_deck();
    auto b = task84::make_deck();

    std::mt19937 rng1(1);
    std::mt19937 rng2(999999);
    task84::shuffle_deck(a, rng1);
    task84::shuffle_deck(b, rng2);

    /* Probabilistic. Two arbitrary 52-card permutations agree with
     * probability 1/52!, which is so small it's effectively 0. */
    EXPECT_NE(a, b);
}

TEST(shuffle, deal_five_gives_five_distinct_cards_in_range)
{
    std::mt19937 rng(7);
    auto hand = task84::deal_five(rng);

    ASSERT_EQ(hand.size(), 5u);
    std::set<int> uniq(hand.begin(), hand.end());
    EXPECT_EQ(uniq.size(), 5u) << "all five cards must be distinct";

    for (int card : hand)
    {
        EXPECT_GE(card, 0);
        EXPECT_LE(card, 51);
    }
}

TEST(shuffle, deal_five_same_seed_same_hand)
{
    std::mt19937 rng1(123);
    std::mt19937 rng2(123);
    auto h1 = task84::deal_five(rng1);
    auto h2 = task84::deal_five(rng2);
    EXPECT_EQ(h1, h2);
}

TEST(shuffle, shuffle_advances_the_engine)
{
    auto d = task84::make_deck();
    std::mt19937 rng(2026);
    const auto before = rng;

    task84::shuffle_deck(d, rng);

    EXPECT_NE(rng, before) << "the engine state should change after shuffle";
}
