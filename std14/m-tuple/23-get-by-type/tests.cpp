#include <gtest/gtest.h>

#include <string>
#include <tuple>

#include "solution.hpp"

namespace t = task23;

TEST(get_by_type, profile_age_is_42)
{
    auto p = t::make_profile();
    EXPECT_EQ(t::get_age(p), 42);
}

TEST(get_by_type, profile_name_is_alice)
{
    auto p = t::make_profile();
    EXPECT_EQ(t::get_name(p), "alice");
}

TEST(get_by_type, profile_score_is_pi)
{
    auto p = t::make_profile();
    EXPECT_DOUBLE_EQ(t::get_score(p), 3.14);
}

TEST(get_by_type, get_int_directly)
{
    /* The scaffold's `get_age` returns int via std::get<int>.
     * Show that get<T> by type works for std::get directly. */
    auto p = std::make_tuple(10, std::string("hi"), 2.5);
    EXPECT_EQ(std::get<int>(p), 10);
}

TEST(get_by_type, get_string_directly)
{
    auto p = std::make_tuple(10, std::string("hi"), 2.5);
    EXPECT_EQ(std::get<std::string>(p), "hi");
}

TEST(get_by_type, get_double_directly)
{
    auto p = std::make_tuple(10, std::string("hi"), 2.5);
    EXPECT_DOUBLE_EQ(std::get<double>(p), 2.5);
}

TEST(get_by_type, modify_via_get_by_type)
{
    /* std::get<T> returns a reference; you can write through it. */
    auto p = std::make_tuple(0, std::string(""), 0.0);
    std::get<int>(p) = 99;
    EXPECT_EQ(std::get<int>(p), 99);
}

TEST(get_by_type, const_tuple_returns_const_ref)
{
    const auto p = std::make_tuple(1, std::string("x"), 1.5);
    int v = std::get<int>(p); /* compiles for const */
    EXPECT_EQ(v, 1);
}

TEST(get_by_type, returns_correct_type)
{
    auto p = std::make_tuple(1, std::string("x"), 1.5);
    auto &name_ref = std::get<std::string>(p);
    EXPECT_TRUE((std::is_same<decltype(name_ref), std::string &>::value));
}
