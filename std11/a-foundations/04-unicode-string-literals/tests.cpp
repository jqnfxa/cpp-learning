#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "solution.hpp"

/*
 * Compile-time guarantees about literal sizes. sizeof gives total
 * bytes (including the null terminator) of an array literal.
 */

static_assert(sizeof(u8"a") == 2, "u8: 1 byte content + 1 byte null");
static_assert(sizeof(u"a") == 4, "u:  1 char16_t content + 1 char16_t null = 4 bytes");
static_assert(sizeof(U"a") == 8, "U:  1 char32_t content + 1 char32_t null = 8 bytes");

/*
 * "héllo" measured at compile time. The é escape produces
 * e-acute regardless of source encoding, so the byte/unit counts
 * below are portable.
 */
static_assert(sizeof(u8"héllo") == 7, "UTF-8 of h-eacute-llo: 6 bytes content + 1 null");
static_assert(sizeof(u"héllo") == 12, "UTF-16: 5 code units * 2 + null * 2");
static_assert(sizeof(U"héllo") == 24, "UTF-32: 5 code units * 4 + null * 4");

/* Raw and escape-processed literals produce identical bytes. */
static_assert(sizeof(R"(abc)") == sizeof("abc"), "raw and regular size match for ASCII");
static_assert(sizeof(R"(\d+)") == sizeof("\\d+"), "raw byte count matches escape-processed");

TEST(unicode_literals, utf8_byte_count)
{
    EXPECT_EQ(std::strlen(task04::as_utf8()), 6u);
}

TEST(unicode_literals, utf8_byte_pattern)
{
    /*
     * UTF-8 bytes of "héllo":
     *   h     -> 0x68
     *   é     -> 0xC3 0xA9
     *   l     -> 0x6C
     *   l     -> 0x6C
     *   o     -> 0x6F
     */
    const unsigned char *s = reinterpret_cast<const unsigned char *>(task04::as_utf8());
    ASSERT_EQ(std::strlen(task04::as_utf8()), 6u);
    EXPECT_EQ(s[0], 0x68u);
    EXPECT_EQ(s[1], 0xC3u);
    EXPECT_EQ(s[2], 0xA9u);
    EXPECT_EQ(s[3], 0x6Cu);
    EXPECT_EQ(s[4], 0x6Cu);
    EXPECT_EQ(s[5], 0x6Fu);
}

TEST(unicode_literals, utf16_code_unit_count)
{
    EXPECT_EQ(std::char_traits<char16_t>::length(task04::as_utf16()), 5u);
}

TEST(unicode_literals, utf16_code_units_match_heallo)
{
    const char16_t *s = task04::as_utf16();
    ASSERT_EQ(std::char_traits<char16_t>::length(s), 5u);
    EXPECT_EQ(s[0], u'h');
    EXPECT_EQ(s[1], u'é');
    EXPECT_EQ(s[2], u'l');
    EXPECT_EQ(s[3], u'l');
    EXPECT_EQ(s[4], u'o');
}

TEST(unicode_literals, utf32_code_unit_count)
{
    EXPECT_EQ(std::char_traits<char32_t>::length(task04::as_utf32()), 5u);
}

TEST(unicode_literals, utf32_code_units_match_heallo)
{
    const char32_t *s = task04::as_utf32();
    ASSERT_EQ(std::char_traits<char32_t>::length(s), 5u);
    EXPECT_EQ(s[0], U'h');
    EXPECT_EQ(s[1], U'é');
    EXPECT_EQ(s[2], U'l');
    EXPECT_EQ(s[3], U'l');
    EXPECT_EQ(s[4], U'o');
}

TEST(unicode_literals, raw_string_matches_escape_processed)
{
    /* Raw R"(\d+\.\d+)" and escape-processed "\\d+\\.\\d+" produce identical bytes. */
    EXPECT_STREQ(task04::as_raw(), "\\d+\\.\\d+");
    EXPECT_EQ(std::strlen(task04::as_raw()), 8u);
}

TEST(unicode_literals, raw_string_byte_pattern)
{
    const char *s = task04::as_raw();
    ASSERT_EQ(std::strlen(s), 8u);
    EXPECT_EQ(s[0], '\\');
    EXPECT_EQ(s[1], 'd');
    EXPECT_EQ(s[2], '+');
    EXPECT_EQ(s[3], '\\');
    EXPECT_EQ(s[4], '.');
    EXPECT_EQ(s[5], '\\');
    EXPECT_EQ(s[6], 'd');
    EXPECT_EQ(s[7], '+');
}
