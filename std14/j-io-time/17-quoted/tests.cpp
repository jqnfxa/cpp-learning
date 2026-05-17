#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

namespace t = task17;

TEST(quoted, serialize_simple_word)
{
    EXPECT_EQ(t::serialize("hello"), "\"hello\"");
}

TEST(quoted, serialize_with_spaces)
{
    EXPECT_EQ(t::serialize("hello world"), "\"hello world\"");
}

TEST(quoted, serialize_embedded_quote)
{
    /* The interior `"` must be escaped with `\`. */
    EXPECT_EQ(t::serialize("say \"hi\""), "\"say \\\"hi\\\"\"");
}

TEST(quoted, serialize_embedded_backslash)
{
    /* The interior `\` must be escaped too. */
    EXPECT_EQ(t::serialize("a\\b"), "\"a\\\\b\"");
}

TEST(quoted, serialize_empty)
{
    EXPECT_EQ(t::serialize(""), "\"\"");
}

TEST(quoted, round_trip_simple)
{
    EXPECT_EQ(t::round_trip("hello"), "hello");
}

TEST(quoted, round_trip_with_spaces)
{
    /* The KEY pedagogical case: naive `>>` would read only
     * "hello"; std::quoted preserves the embedded space. */
    EXPECT_EQ(t::round_trip("hello world"), "hello world");
}

TEST(quoted, round_trip_multiple_spaces)
{
    EXPECT_EQ(t::round_trip("a   b   c"), "a   b   c");
}

TEST(quoted, round_trip_embedded_quote)
{
    EXPECT_EQ(t::round_trip("say \"hi\""), "say \"hi\"");
}

TEST(quoted, round_trip_empty)
{
    EXPECT_EQ(t::round_trip(""), "");
}

TEST(quoted, round_trip_tab_and_punctuation)
{
    EXPECT_EQ(t::round_trip("\tfoo, bar; baz!"), "\tfoo, bar; baz!");
}

TEST(quoted, round_trip_unicode_bytes)
{
    /* Non-ASCII bytes are NOT escaped -- they pass through. */
    std::string s = "caf\xc3\xa9"; /* "café" in UTF-8 */
    EXPECT_EQ(t::round_trip(s), s);
}
