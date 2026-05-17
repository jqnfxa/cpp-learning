#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

TEST(is_permutation, classic_anagram)
{
    EXPECT_TRUE(task89::is_anagram("listen", "silent"));
}

TEST(is_permutation, not_anagram)
{
    EXPECT_FALSE(task89::is_anagram("hello", "world"));
}

TEST(is_permutation, identical_strings_are_anagrams)
{
    EXPECT_TRUE(task89::is_anagram("abc", "abc"));
}

TEST(is_permutation, empty_strings)
{
    EXPECT_TRUE(task89::is_anagram("", ""));
}

TEST(is_permutation, length_mismatch_returns_false)
{
    /* The C++11 3-iterator overload's precondition would be violated
     * if we called it without the length guard. is_anagram must return
     * false here, not invoke UB. */
    EXPECT_FALSE(task89::is_anagram("abc", "ab"));
    EXPECT_FALSE(task89::is_anagram("ab", "abc"));
    EXPECT_FALSE(task89::is_anagram("a", ""));
    EXPECT_FALSE(task89::is_anagram("", "x"));
}

TEST(is_permutation, repeats_count_correctly)
{
    EXPECT_TRUE(task89::is_anagram("aabb", "abab"));
    EXPECT_TRUE(task89::is_anagram("aabb", "bbaa"));
    EXPECT_FALSE(task89::is_anagram("aabb", "abbb"));
}

TEST(is_permutation, case_sensitive)
{
    /* Case-sensitive: capital L is not lowercase l. */
    EXPECT_FALSE(task89::is_anagram("Listen", "silent"));
    /* "Listen" letters: L, i, s, t, e, n.  Permuting preserves the
     * capital L. */
    EXPECT_TRUE(task89::is_anagram("Listen", "nLites"));
}

TEST(is_permutation, whitespace_is_a_character)
{
    /* Spaces participate in the multiset. */
    EXPECT_FALSE(task89::is_anagram("ab c", "abc"));
    EXPECT_TRUE(task89::is_anagram("a b", "b a"));
}

TEST(is_permutation, longer_anagram_pair)
{
    EXPECT_TRUE(task89::is_anagram("conversation", "voicesranton"));
}

TEST(is_permutation, single_character)
{
    EXPECT_TRUE(task89::is_anagram("a", "a"));
    EXPECT_FALSE(task89::is_anagram("a", "b"));
}
