#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

using namespace task43;

TEST(udl_string, bytes_empty)
{
    auto v = ""_bytes;
    EXPECT_TRUE(v.empty());
}

TEST(udl_string, bytes_single_char)
{
    auto v = "A"_bytes;
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], 0x41u);
}

TEST(udl_string, bytes_two_chars)
{
    auto v = "AB"_bytes;
    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 0x41u);
    EXPECT_EQ(v[1], 0x42u);
}

TEST(udl_string, bytes_hex_escapes)
{
    auto v = "\x01\xff"_bytes;
    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 0x01u);
    EXPECT_EQ(v[1], 0xffu);
}

TEST(udl_string, bytes_with_embedded_null)
{
    /* "a\0b" has 3 chars: 'a', '\0', 'b'. */
    auto v = "a\0b"_bytes;
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 'a');
    EXPECT_EQ(v[1], 0u);
    EXPECT_EQ(v[2], 'b');
}

TEST(udl_string, len_empty)
{
    EXPECT_EQ(""_len, 0u);
}

TEST(udl_string, len_hello)
{
    EXPECT_EQ("hello"_len, 5u);
}

TEST(udl_string, len_with_embedded_null)
{
    EXPECT_EQ("a\0b"_len, 3u);
}

TEST(udl_string, len_long_string)
{
    EXPECT_EQ("the quick brown fox jumps over the lazy dog"_len, 43u);
}

TEST(udl_string, return_types)
{
    static_assert(std::is_same<decltype("x"_bytes), std::vector<unsigned char>>::value, "_bytes returns vector<unsigned char>");
    static_assert(std::is_same<decltype("x"_len), std::size_t>::value, "_len returns size_t");
    SUCCEED();
}
