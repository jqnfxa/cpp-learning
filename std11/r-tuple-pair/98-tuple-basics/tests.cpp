#include <gtest/gtest.h>

#include <string>
#include <tuple>

#include "solution.hpp"

TEST(tuple_basics, make_record_basic)
{
    auto r = task98::make_record(42, "alice", 3.14);
    EXPECT_EQ(std::get<0>(r), 42);
    EXPECT_EQ(std::get<1>(r), "alice");
    EXPECT_DOUBLE_EQ(std::get<2>(r), 3.14);
}

TEST(tuple_basics, make_record_negative_id)
{
    auto r = task98::make_record(-1, "bob", 0.0);
    EXPECT_EQ(std::get<0>(r), -1);
    EXPECT_EQ(std::get<1>(r), "bob");
    EXPECT_DOUBLE_EQ(std::get<2>(r), 0.0);
}

TEST(tuple_basics, make_record_empty_name)
{
    auto r = task98::make_record(0, "", -2.5);
    EXPECT_EQ(std::get<0>(r), 0);
    EXPECT_EQ(std::get<1>(r), "");
    EXPECT_DOUBLE_EQ(std::get<2>(r), -2.5);
}

TEST(tuple_basics, tuple_type_size_is_three)
{
    EXPECT_EQ(std::tuple_size<task98::record>::value, 3u);
}

TEST(tuple_basics, tuple_element_types_match)
{
    using e0 = std::tuple_element<0, task98::record>::type;
    using e1 = std::tuple_element<1, task98::record>::type;
    using e2 = std::tuple_element<2, task98::record>::type;
    static_assert(std::is_same<e0, int>::value, "element 0 must be int");
    static_assert(std::is_same<e1, std::string>::value, "element 1 must be std::string");
    static_assert(std::is_same<e2, double>::value, "element 2 must be double");
    SUCCEED();
}

TEST(tuple_basics, two_records_compare)
{
    auto a = task98::make_record(1, "alice", 1.0);
    auto b = task98::make_record(1, "alice", 1.0);
    auto c = task98::make_record(2, "alice", 1.0);

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_LT(a, c); /* lexicographic, first slot differs */
}

TEST(tuple_basics, returned_record_is_modifiable_via_get)
{
    auto r = task98::make_record(1, "alice", 1.0);
    std::get<0>(r) = 99;
    std::get<1>(r) = "carol";
    EXPECT_EQ(std::get<0>(r), 99);
    EXPECT_EQ(std::get<1>(r), "carol");
}
