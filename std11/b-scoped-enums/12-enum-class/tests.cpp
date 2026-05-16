#include <gtest/gtest.h>

#include <cstring>
#include <type_traits>

#include "solution.hpp"

/* color must not be implicitly convertible to int -- the whole point. */
static_assert(!std::is_convertible<task12::color, int>::value, "enum class color must not implicitly convert to int");

/* Default underlying type for an unspecified scoped enum is int. */
static_assert(std::is_same<std::underlying_type<task12::color>::type, int>::value, "default underlying type of enum class is int");
static_assert(sizeof(task12::color) == sizeof(int), "sizeof matches the underlying type");

TEST(enum_class, to_string_red)
{
    EXPECT_STREQ(task12::to_string(task12::color::red), "red");
}

TEST(enum_class, to_string_green)
{
    EXPECT_STREQ(task12::to_string(task12::color::green), "green");
}

TEST(enum_class, to_string_blue)
{
    EXPECT_STREQ(task12::to_string(task12::color::blue), "blue");
}

TEST(enum_class, scoped_enumerators_are_distinct)
{
    EXPECT_NE(task12::color::red, task12::color::green);
    EXPECT_NE(task12::color::green, task12::color::blue);
    EXPECT_NE(task12::color::blue, task12::color::red);
}

TEST(enum_class, explicit_cast_to_int)
{
    /*
     * No *implicit* conversion -- the static_assert above pins that --
     * but static_cast still works. Default enumerator values run
     * 0, 1, 2 ... in declaration order.
     */
    EXPECT_EQ(static_cast<int>(task12::color::red), 0);
    EXPECT_EQ(static_cast<int>(task12::color::green), 1);
    EXPECT_EQ(static_cast<int>(task12::color::blue), 2);
}
