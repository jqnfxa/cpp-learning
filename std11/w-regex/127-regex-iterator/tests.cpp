#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

TEST(regex_iterator, basic_sentence)
{
    EXPECT_EQ(task127::tokenize_words("hello world"),
              (std::vector<std::string>{"hello", "world"}));
}

TEST(regex_iterator, four_words)
{
    EXPECT_EQ(task127::tokenize_words("the quick brown fox"),
              (std::vector<std::string>{"the", "quick", "brown", "fox"}));
}

TEST(regex_iterator, multiple_spaces_collapsed)
{
    /* \w+ doesn't match spaces; multiple spaces between tokens are
     * just separators. */
    EXPECT_EQ(task127::tokenize_words("  hello   world  "),
              (std::vector<std::string>{"hello", "world"}));
}

TEST(regex_iterator, tabs_and_newlines)
{
    EXPECT_EQ(task127::tokenize_words("alpha\tbeta\ngamma"),
              (std::vector<std::string>{"alpha", "beta", "gamma"}));
}

TEST(regex_iterator, punctuation_is_separator)
{
    EXPECT_EQ(task127::tokenize_words("hello, world!"),
              (std::vector<std::string>{"hello", "world"}));
}

TEST(regex_iterator, mixed_punctuation)
{
    EXPECT_EQ(task127::tokenize_words("punctuation; like: this!"),
              (std::vector<std::string>{"punctuation", "like", "this"}));
}

TEST(regex_iterator, digits_are_word_chars)
{
    EXPECT_EQ(task127::tokenize_words("abc 123 def"),
              (std::vector<std::string>{"abc", "123", "def"}));
}

TEST(regex_iterator, alphanumeric_tokens)
{
    EXPECT_EQ(task127::tokenize_words("id42 v3 user_99"),
              (std::vector<std::string>{"id42", "v3", "user_99"}));
}

TEST(regex_iterator, empty_input)
{
    EXPECT_EQ(task127::tokenize_words(""), std::vector<std::string>{});
}

TEST(regex_iterator, only_separators)
{
    EXPECT_EQ(task127::tokenize_words("   ,!?  "), std::vector<std::string>{});
}

TEST(regex_iterator, single_word)
{
    EXPECT_EQ(task127::tokenize_words("only"), std::vector<std::string>{"only"});
}
