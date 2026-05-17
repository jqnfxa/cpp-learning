#pragma once

#include <algorithm>
#include <random>
#include <vector>

/*
 * Step 84 -- std::shuffle. See task.md for the spec.
 *
 * Implement make_deck / shuffle_deck / deal_five over a 52-int
 * deck using std::mt19937 as the URBG.
 */

namespace task84
{

using deck = std::vector<int>;

/* TODO: fill a vector with 0..51 (a for-loop or std::iota). */
inline deck make_deck()
{
    return {};
}

/* TODO: std::shuffle(d.begin(), d.end(), rng); */
inline void shuffle_deck(deck & /*d*/, std::mt19937 & /*rng*/)
{
}

/* TODO: make a deck, shuffle it with rng, return the first 5 cards. */
inline std::vector<int> deal_five(std::mt19937 & /*rng*/)
{
    return {};
}

}
