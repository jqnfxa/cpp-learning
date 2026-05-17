#include <gtest/gtest.h>
#include <string>

#include "solution.hpp"

TEST(StringViewSuite, three_words)
{
    EXPECT_EQ(task33::count_words("hello world foo"), 3u);
}

TEST(StringViewSuite, single_word)
{
    EXPECT_EQ(task33::count_words("alone"), 1u);
}

TEST(StringViewSuite, empty)
{
    EXPECT_EQ(task33::count_words(""), 0u);
}

TEST(StringViewSuite, multiple_spaces)
{
    EXPECT_EQ(task33::count_words("  hello   world  "), 2u);
}

TEST(StringViewSuite, only_whitespace)
{
    EXPECT_EQ(task33::count_words("   \t\n  "), 0u);
}

TEST(StringViewSuite, takes_std_string_arg)
{
    std::string s = "a b c";
    EXPECT_EQ(task33::count_words(s), 3u);
}

TEST(StringViewSuite, takes_literal_arg)
{
    auto n = task33::count_words("one two");
    EXPECT_EQ(n, 2u);
}
