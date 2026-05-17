#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "solution.hpp"

namespace t = task19;
using namespace std::string_literals;

TEST(string_udl, hello_world_value)
{
    EXPECT_EQ(t::hello_world(), "hello world");
}

TEST(string_udl, hello_world_size)
{
    EXPECT_EQ(t::hello_world().size(), 11u);
}

TEST(string_udl, greeting_alice)
{
    EXPECT_EQ(t::greeting("alice"), "Hello, alice!");
}

TEST(string_udl, greeting_empty)
{
    EXPECT_EQ(t::greeting(""), "Hello, !");
}

TEST(string_udl, greeting_world)
{
    EXPECT_EQ(t::greeting("world"), "Hello, world!");
}

TEST(string_udl, size_of_hello_is_5)
{
    /* "hello"s is std::string("hello"); size() == 5.
     * sizeof("hello") (without UDL) is 6 (includes null). */
    EXPECT_EQ(t::size_of_hello(), 5u);
}

TEST(string_udl, plain_s_suffix_yields_string)
{
    auto x = "abc"s;
    EXPECT_TRUE((std::is_same<decltype(x), std::string>::value));
}

TEST(string_udl, plain_s_supports_concat_with_literal)
{
    /* The UDL form makes std::string + const char* work
     * naturally. The pedagogical example. */
    auto x = "hello"s + " world";
    EXPECT_EQ(x, "hello world");
    EXPECT_EQ(x.size(), 11u);
}

TEST(string_udl, s_suffix_handles_embedded_null)
{
    /* "abc\0def"s is a 7-character std::string, NOT truncated
     * at the null. The const char* form would truncate. */
    auto x = std::string("abc\0def", 7);
    EXPECT_EQ(x.size(), 7u);
}
