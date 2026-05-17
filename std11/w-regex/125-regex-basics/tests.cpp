#include <gtest/gtest.h>

#include "solution.hpp"

TEST(regex_basics, simple_address)
{
    EXPECT_TRUE(task125::is_email_shaped("alice@example.com"));
}

TEST(regex_basics, multi_level_domain)
{
    EXPECT_TRUE(task125::is_email_shaped("bob.smith@foo.bar.org"));
}

TEST(regex_basics, plus_tag)
{
    EXPECT_TRUE(task125::is_email_shaped("user+tag@domain.io"));
}

TEST(regex_basics, digits_in_local_and_domain)
{
    EXPECT_TRUE(task125::is_email_shaped("user123@server42.example.net"));
}

TEST(regex_basics, no_at_symbol)
{
    EXPECT_FALSE(task125::is_email_shaped("alice"));
}

TEST(regex_basics, empty_local_part)
{
    EXPECT_FALSE(task125::is_email_shaped("@example.com"));
}

TEST(regex_basics, no_tld)
{
    EXPECT_FALSE(task125::is_email_shaped("alice@example"));
}

TEST(regex_basics, single_char_tld)
{
    /* Pattern requires 2+ letter TLD. */
    EXPECT_FALSE(task125::is_email_shaped("alice@example.x"));
}

TEST(regex_basics, empty_string)
{
    EXPECT_FALSE(task125::is_email_shaped(""));
}

TEST(regex_basics, trailing_text)
{
    /* regex_match requires the WHOLE string to match. */
    EXPECT_FALSE(task125::is_email_shaped("alice@example.com extra"));
}

TEST(regex_basics, leading_text)
{
    EXPECT_FALSE(task125::is_email_shaped("hello alice@example.com"));
}
