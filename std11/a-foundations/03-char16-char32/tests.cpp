#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

/* Language-level guarantees about the new char types, pinned at compile time. */
static_assert(sizeof(char16_t) >= 2, "C++11: char16_t is at least 16 bits wide");
static_assert(sizeof(char32_t) >= 4, "C++11: char32_t is at least 32 bits wide");

/* The new char types are distinct from char, wchar_t, and from each other. */
static_assert(!std::is_same<char16_t, char>::value, "char16_t is not char");
static_assert(!std::is_same<char16_t, wchar_t>::value, "char16_t is not wchar_t");
static_assert(!std::is_same<char32_t, char>::value, "char32_t is not char");
static_assert(!std::is_same<char32_t, wchar_t>::value, "char32_t is not wchar_t");
static_assert(!std::is_same<char16_t, char32_t>::value, "char16_t is not char32_t");

TEST(char16_char32, empty_strings)
{
    EXPECT_EQ(task03::count_utf16_units(u""), 0u);
    EXPECT_EQ(task03::count_utf32_units(U""), 0u);
}

TEST(char16_char32, ascii)
{
    EXPECT_EQ(task03::count_utf16_units(u"abc"), 3u);
    EXPECT_EQ(task03::count_utf32_units(U"abc"), 3u);
    EXPECT_EQ(task03::count_utf16_units(u"hello, world"), 12u);
}

TEST(char16_char32, bmp_non_ascii)
{
    /* U+2605 BLACK STAR sits inside the BMP, so UTF-16 fits it in 1 unit. */
    EXPECT_EQ(task03::count_utf16_units(u"a★b"), 3u);
    EXPECT_EQ(task03::count_utf32_units(U"a★b"), 3u);
}

TEST(char16_char32, supplementary_plane_surrogate_pair)
{
    /*
     * U+1F389 PARTY POPPER is above U+FFFF, so UTF-16 needs a
     * surrogate pair (2 code units for one Unicode code point).
     * UTF-32 stores it as a single code unit.
     */
    EXPECT_EQ(task03::count_utf16_units(u"\U0001F389"), 2u);
    EXPECT_EQ(task03::count_utf32_units(U"\U0001F389"), 1u);
}

TEST(char16_char32, mixed_ascii_and_supplementary)
{
    /* "a🎉b" -- 'a' (1) + supplementary (2 in UTF-16, 1 in UTF-32) + 'b' (1). */
    EXPECT_EQ(task03::count_utf16_units(u"a\U0001F389b"), 4u);
    EXPECT_EQ(task03::count_utf32_units(U"a\U0001F389b"), 3u);
}
