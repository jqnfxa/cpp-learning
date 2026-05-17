#include <gtest/gtest.h>

#include <algorithm>
#include <set>
#include <vector>

#include "solution.hpp"

namespace t = task24;

TEST(shuffle, preserves_size)
{
    std::vector<int> v{1, 2, 3, 4, 5};
    auto s = t::shuffle_copy(v, 42);
    EXPECT_EQ(s.size(), 5u);
}

TEST(shuffle, preserves_elements)
{
    std::vector<int> v{1, 2, 3, 4, 5};
    auto s = t::shuffle_copy(v, 42);
    std::sort(s.begin(), s.end());
    EXPECT_EQ(s, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(shuffle, actually_shuffles)
{
    /* With a non-trivial seed, the order should change at
     * least once across larger ranges. The probability of
     * identity permutation for size 20 is 1/20! ~ 4e-19. */
    std::vector<int> v;
    for (int i = 0; i < 20; ++i)
    {
        v.push_back(i);
    }
    auto s = t::shuffle_copy(v, 12345);
    EXPECT_NE(s, v);
}

TEST(shuffle, deterministic_with_seed)
{
    /* Same seed -> same permutation. */
    std::vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i);
    }
    auto a = t::shuffle_copy(v, 999);
    auto b = t::shuffle_copy(v, 999);
    EXPECT_EQ(a, b);
}
