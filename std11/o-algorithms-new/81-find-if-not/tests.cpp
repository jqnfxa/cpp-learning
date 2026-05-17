#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

TEST(find_if_not, first_non_whitespace_leading_space)
{
    EXPECT_EQ(task81::first_non_whitespace("  hello"), 2u);
}

TEST(find_if_not, first_non_whitespace_no_leading)
{
    EXPECT_EQ(task81::first_non_whitespace("abc"), 0u);
}

TEST(find_if_not, first_non_whitespace_all_whitespace)
{
    EXPECT_EQ(task81::first_non_whitespace("   "), 3u);
    EXPECT_EQ(task81::first_non_whitespace("\t\n\r "), 4u);
}

TEST(find_if_not, first_non_whitespace_empty_returns_zero)
{
    /* Empty string -> s.size() == 0. */
    EXPECT_EQ(task81::first_non_whitespace(""), 0u);
}

TEST(find_if_not, first_non_whitespace_mixed_whitespace_kinds)
{
    EXPECT_EQ(task81::first_non_whitespace("\t\n  x"), 4u);
    EXPECT_EQ(task81::first_non_whitespace("\v\f\rxy"), 3u);
}

TEST(find_if_not, first_non_whitespace_internal_whitespace_ignored)
{
    /* Internal whitespace after the first non-whitespace doesn't move
     * the answer. */
    EXPECT_EQ(task81::first_non_whitespace("  a b c"), 2u);
}

TEST(find_if_not, trim_front_basic)
{
    EXPECT_EQ(task81::trim_front("  hello"), "hello");
    EXPECT_EQ(task81::trim_front("abc"), "abc");
}

TEST(find_if_not, trim_front_all_whitespace)
{
    EXPECT_EQ(task81::trim_front("   "), "");
    EXPECT_EQ(task81::trim_front("\t\n\r"), "");
}

TEST(find_if_not, trim_front_empty)
{
    EXPECT_EQ(task81::trim_front(""), "");
}

TEST(find_if_not, trim_front_preserves_trailing_whitespace)
{
    /* find_if_not only trims the LEADING run. */
    EXPECT_EQ(task81::trim_front("  hello   "), "hello   ");
}

TEST(find_if_not, trim_front_high_byte_chars_not_treated_as_whitespace)
{
    /* Bytes with the high bit set (e.g. UTF-8 continuation bytes,
     * Latin-1 letters when stored in a char) must not be misclassified
     * by passing a negative signed char into std::isspace. The
     * unsigned-char-cast lambda avoids that UB. */
    std::string s;
    s.push_back(static_cast<char>(0xc3));
    s.push_back(static_cast<char>(0xa9)); /* "é" in UTF-8 */
    s += " rest";
    EXPECT_EQ(task81::first_non_whitespace(s), 0u);
}
