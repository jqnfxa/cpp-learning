#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "solution.hpp"

namespace t = task07;

TEST(ctad, pair_values_correct)
{
    auto p = t::make_pair_via_ctad(1, 2.5);
    EXPECT_EQ(p.first, 1);
    EXPECT_DOUBLE_EQ(p.second, 2.5);
}

TEST(ctad, pair_negative_values)
{
    auto p = t::make_pair_via_ctad(-7, -1.5);
    EXPECT_EQ(p.first, -7);
    EXPECT_DOUBLE_EQ(p.second, -1.5);
}

TEST(ctad, vector_values_in_order)
{
    auto v = t::make_vector_via_ctad(10, 20, 30);
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(ctad, vector_negative_values)
{
    auto v = t::make_vector_via_ctad(-1, -2, -3);
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], -1);
    EXPECT_EQ(v[1], -2);
    EXPECT_EQ(v[2], -3);
}

TEST(ctad, tuple_age_name_score)
{
    auto tu = t::make_tuple_via_ctad(30, "alice", 3.14);
    EXPECT_EQ(std::get<0>(tu), 30);
    EXPECT_EQ(std::get<1>(tu), "alice");
    EXPECT_DOUBLE_EQ(std::get<2>(tu), 3.14);
}

TEST(ctad, tuple_other_values)
{
    auto tu = t::make_tuple_via_ctad(42, "bob", 1.5);
    EXPECT_EQ(std::get<0>(tu), 42);
    EXPECT_EQ(std::get<1>(tu), "bob");
    EXPECT_DOUBLE_EQ(std::get<2>(tu), 1.5);
}

TEST(ctad, direct_pair_deduction)
{
    /* The textbook usage: `std::pair p{1, 2.5}` deduces to
     * pair<int, double>. */
    std::pair p{1, 2.5};
    EXPECT_TRUE((std::is_same<decltype(p), std::pair<int, double>>::value));
    EXPECT_EQ(p.first, 1);
    EXPECT_DOUBLE_EQ(p.second, 2.5);
}

TEST(ctad, direct_vector_deduction)
{
    std::vector v{1, 2, 3, 4, 5};
    EXPECT_TRUE((std::is_same<decltype(v), std::vector<int>>::value));
    EXPECT_EQ(v.size(), 5u);
}

TEST(ctad, direct_tuple_deduction)
{
    std::tuple tu{1, std::string("x"), 2.5};
    EXPECT_TRUE((std::is_same<decltype(tu),
                              std::tuple<int, std::string, double>>::value));
}

TEST(ctad, pair_from_strings)
{
    std::pair p{std::string("a"), std::string("b")};
    EXPECT_EQ(p.first, "a");
    EXPECT_EQ(p.second, "b");
}
