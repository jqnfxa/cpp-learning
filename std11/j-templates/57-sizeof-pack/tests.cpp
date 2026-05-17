#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

/* Compile-time -- sizeof... yields a constant expression. */
static_assert(task57::count_args() == 0, "no args");
static_assert(task57::count_args(1) == 1, "one arg");
static_assert(task57::count_args(1, 'x', 3.14) == 3, "three args");

static_assert(task57::count_types<>() == 0, "no type args");
static_assert(task57::count_types<int>() == 1, "one type arg");
static_assert(task57::count_types<int, double, char>() == 3, "three type args");

static_assert(task57::type_list<>::size == 0, "empty type_list");
static_assert(task57::type_list<int>::size == 1, "one type");
static_assert(task57::type_list<int, double, char, std::string>::size == 4, "four types");

TEST(sizeof_pack, count_args_runtime)
{
    EXPECT_EQ(task57::count_args(), 0u);
    EXPECT_EQ(task57::count_args(42), 1u);
    EXPECT_EQ(task57::count_args("a", "b"), 2u);
    EXPECT_EQ(task57::count_args(1, 2.0, "x", 'c'), 4u);
}

TEST(sizeof_pack, count_types_runtime)
{
    EXPECT_EQ(task57::count_types<>(), 0u);
    EXPECT_EQ(task57::count_types<int>(), 1u);
    EXPECT_EQ((task57::count_types<int, double>()), 2u);
}

TEST(sizeof_pack, type_list_size_runtime)
{
    /*
     * `+x` forces a temporary, avoiding odr-use of the static
     * constexpr member -- which otherwise requires an out-of-class
     * definition in C++11.
     */
    EXPECT_EQ(+task57::type_list<>::size, 0u);
    EXPECT_EQ(+task57::type_list<int>::size, 1u);
    EXPECT_EQ((+task57::type_list<int, double, char>::size), 3u);
}
