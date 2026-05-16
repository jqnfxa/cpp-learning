#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "solution.hpp"

/* Compile-time identity for several instantiations of each alias template. */

static_assert(std::is_same<task09::vec<int>, std::vector<int>>::value, "vec<int> must alias std::vector<int>");
static_assert(std::is_same<task09::vec<std::string>, std::vector<std::string>>::value, "vec<string> must alias std::vector<string>");

static_assert(std::is_same<task09::map<int, std::string>, std::map<int, std::string>>::value, "map<int,string> must alias std::map<int,string>");
static_assert(std::is_same<task09::map<std::string, int>, std::map<std::string, int>>::value, "map<string,int> must alias std::map<string,int>");

static_assert(std::is_same<task09::array<int, 5>, std::array<int, 5>>::value, "array<int,5> must alias std::array<int,5>");
static_assert(std::is_same<task09::array<double, 16>, std::array<double, 16>>::value, "array<double,16> must alias std::array<double,16>");

static_assert(std::is_same<task09::ptr<int>, int *>::value, "ptr<int> must alias int*");
static_assert(std::is_same<task09::ptr<const char>, const char *>::value, "ptr<const char> must alias const char*");

/* Runtime: each alias instantiates and behaves like the underlying type. */

TEST(alias_template, vec_is_usable)
{
    task09::vec<int> v = {10, 20, 30};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], 20);
}

TEST(alias_template, map_is_usable)
{
    task09::map<std::string, int> m{{"one", 1}, {"two", 2}};
    EXPECT_EQ(m["one"], 1);
    EXPECT_EQ(m["two"], 2);
    EXPECT_EQ(m.size(), 2u);
}

TEST(alias_template, array_is_usable)
{
    task09::array<int, 4> a = {{1, 2, 3, 4}};
    EXPECT_EQ(a.size(), 4u);
    EXPECT_EQ(a[3], 4);
}

TEST(alias_template, ptr_is_a_raw_pointer)
{
    int x = 42;
    task09::ptr<int> p = &x;
    EXPECT_EQ(*p, 42);
    *p = 100;
    EXPECT_EQ(x, 100);
}
