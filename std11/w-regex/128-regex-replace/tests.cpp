#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

TEST(regex_replace, single_digit)
{
    EXPECT_EQ(task128::redact_digits("a1b"), "a#b");
}

TEST(regex_replace, all_digits)
{
    EXPECT_EQ(task128::redact_digits("12345"), "#####");
}

TEST(regex_replace, no_digits_passthrough)
{
    EXPECT_EQ(task128::redact_digits("hello world"), "hello world");
}

TEST(regex_replace, mixed_letters_digits)
{
    EXPECT_EQ(task128::redact_digits("abc123def456"), "abc###def###");
}

TEST(regex_replace, empty_input)
{
    EXPECT_EQ(task128::redact_digits(""), "");
}

TEST(regex_replace, phone_number_pattern)
{
    EXPECT_EQ(task128::redact_digits("555-867-5309"), "###-###-####");
}

TEST(regex_replace, leading_digits)
{
    EXPECT_EQ(task128::redact_digits("42is the answer"), "##is the answer");
}

TEST(regex_replace, trailing_digits)
{
    EXPECT_EQ(task128::redact_digits("answer is 42"), "answer is ##");
}

TEST(regex_replace, only_zero)
{
    EXPECT_EQ(task128::redact_digits("0"), "#");
}

TEST(regex_replace, digits_with_punctuation)
{
    EXPECT_EQ(task128::redact_digits("v1.2.3"), "v#.#.#");
}

TEST(regex_replace, preserves_special_characters)
{
    /* '$', '\', '&' have special meaning in the replacement format string
     * (referencing match/captures). Here we replace WITH a plain "#", so
     * none of those should activate. The INPUT still contains them and
     * must be preserved verbatim around the digits. */
    EXPECT_EQ(task128::redact_digits("$1 & \\2"), "$# & \\#");
}

TEST(regex_replace, unicode_letters_unaffected)
{
    /* Non-ASCII bytes aren't \d. They pass through. */
    EXPECT_EQ(task128::redact_digits("\xc3\xa9 9 \xc3\xa9"),
              "\xc3\xa9 # \xc3\xa9");
}
