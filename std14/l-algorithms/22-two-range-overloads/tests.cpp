#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

namespace t = task22;

TEST(two_range_overloads, equal_same_size_equal)
{
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 3};
    EXPECT_TRUE(t::safe_equal(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, equal_same_size_different)
{
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 4};
    EXPECT_FALSE(t::safe_equal(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, equal_prefix_is_NOT_equal)
{
    /* The KEY test: with the C++11 three-arg form, [1,2,3]
     * compares equal to [1,2,3,4,5] because the second range
     * is just assumed to be long enough. The C++14 four-arg
     * form returns false because lengths differ. */
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 3, 4, 5};
    EXPECT_FALSE(t::safe_equal(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, equal_shorter_b_is_not_equal)
{
    /* Symmetric: shorter b. With C++14 returns false; with
     * C++11 it would read past the end of b (UB). */
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{1, 2, 3};
    EXPECT_FALSE(t::safe_equal(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, equal_empty_both)
{
    std::vector<int> a;
    std::vector<int> b;
    EXPECT_TRUE(t::safe_equal(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, equal_empty_a_nonempty_b)
{
    std::vector<int> a;
    std::vector<int> b{1};
    EXPECT_FALSE(t::safe_equal(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, both_match_completely_same)
{
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 3};
    EXPECT_TRUE(t::both_ranges_match_completely(a.begin(), a.end(),
                                                b.begin(), b.end()));
}

TEST(two_range_overloads, both_match_completely_a_shorter)
{
    /* a is a prefix of b -- they match through a's end, but b
     * still has elements. The C++14 form returns false (b not
     * exhausted). C++11 form would return true. */
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 3, 4};
    EXPECT_FALSE(t::both_ranges_match_completely(a.begin(), a.end(),
                                                 b.begin(), b.end()));
}

TEST(two_range_overloads, perm_same_set)
{
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{3, 1, 2};
    EXPECT_TRUE(t::safe_perm(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, perm_different_sizes)
{
    /* Different lengths -> not a permutation. C++14 form
     * checks length first; C++11 form is UB. */
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 3, 4};
    EXPECT_FALSE(t::safe_perm(a.begin(), a.end(), b.begin(), b.end()));
}

TEST(two_range_overloads, perm_disjoint)
{
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    EXPECT_FALSE(t::safe_perm(a.begin(), a.end(), b.begin(), b.end()));
}
