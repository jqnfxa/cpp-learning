#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <utility>

#include "solution.hpp"

TEST(make_and_forward_as_tuple, via_make_tuple_basic)
{
    auto t = task100::via_make_tuple(42, 3.14, "alpha");
    EXPECT_EQ(std::get<0>(t), 42);
    EXPECT_DOUBLE_EQ(std::get<1>(t), 3.14);
    EXPECT_EQ(std::get<2>(t), "alpha");
}

TEST(make_and_forward_as_tuple, via_make_tuple_types_decay)
{
    /* make_tuple decays its arguments to value types. For an std::string
     * passed by value, the tuple element is plain std::string (not
     * reference). The static_assert pins that. */
    auto t = task100::via_make_tuple(0, 0.0, std::string("x"));
    using t_t = decltype(t);
    static_assert(std::is_same<std::tuple_element<0, t_t>::type, int>::value,
                  "element 0 is int");
    static_assert(std::is_same<std::tuple_element<1, t_t>::type, double>::value,
                  "element 1 is double");
    static_assert(std::is_same<std::tuple_element<2, t_t>::type, std::string>::value,
                  "element 2 is std::string");
    SUCCEED();
}

TEST(make_and_forward_as_tuple, piecewise_constructs_point_and_label)
{
    auto p = task100::via_piecewise(3, 4, "tag", 0.75);
    EXPECT_EQ(p.first.x, 3);
    EXPECT_EQ(p.first.y, 4);
    EXPECT_EQ(p.second.text, "tag");
    EXPECT_DOUBLE_EQ(p.second.weight, 0.75);
}

TEST(make_and_forward_as_tuple, piecewise_handles_empty_label_text)
{
    auto p = task100::via_piecewise(0, 0, "", 0.0);
    EXPECT_EQ(p.first.x, 0);
    EXPECT_EQ(p.first.y, 0);
    EXPECT_EQ(p.second.text, "");
    EXPECT_DOUBLE_EQ(p.second.weight, 0.0);
}

TEST(make_and_forward_as_tuple, piecewise_negatives)
{
    auto p = task100::via_piecewise(-1, -2, "neg", -0.5);
    EXPECT_EQ(p.first.x, -1);
    EXPECT_EQ(p.first.y, -2);
    EXPECT_EQ(p.second.text, "neg");
    EXPECT_DOUBLE_EQ(p.second.weight, -0.5);
}

TEST(make_and_forward_as_tuple, piecewise_long_label_text)
{
    /* Long string -> heap allocation. Piecewise should still produce
     * the right value, regardless of the internal moves. */
    std::string long_text(64, 'a');
    auto p = task100::via_piecewise(1, 2, long_text, 1.0);
    EXPECT_EQ(p.second.text, long_text);
    EXPECT_EQ(p.second.text.size(), 64u);
}

TEST(make_and_forward_as_tuple, returned_pair_is_pair_of_point_and_label)
{
    auto p = task100::via_piecewise(1, 2, "x", 1.0);
    using pt = decltype(p);
    static_assert(std::is_same<pt, std::pair<task100::Point, task100::Label>>::value,
                  "must be pair<Point, Label>");
    SUCCEED();
}
