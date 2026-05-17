#include <gtest/gtest.h>

#include <string>
#include <tuple>

#include "solution.hpp"

namespace
{
const task99::record kSample{42, "alice", 3.14};
}

TEST(get_and_tie, first_field)
{
    EXPECT_EQ(task99::first_field(kSample), 42);
}

TEST(get_and_tie, second_field)
{
    EXPECT_EQ(task99::second_field(kSample), "alice");
}

TEST(get_and_tie, third_field)
{
    EXPECT_DOUBLE_EQ(task99::third_field(kSample), 3.14);
}

TEST(get_and_tie, unpack_basic)
{
    int id = 0;
    std::string name;
    double score = 0.0;
    task99::unpack(kSample, id, name, score);
    EXPECT_EQ(id, 42);
    EXPECT_EQ(name, "alice");
    EXPECT_DOUBLE_EQ(score, 3.14);
}

TEST(get_and_tie, unpack_overwrites_existing_values)
{
    int id = 99;
    std::string name = "previous";
    double score = -1.0;
    task99::unpack(kSample, id, name, score);
    EXPECT_EQ(id, 42);
    EXPECT_EQ(name, "alice");
    EXPECT_DOUBLE_EQ(score, 3.14);
}

TEST(get_and_tie, unpack_into_different_record)
{
    task99::record other{-1, "bob", 0.0};
    int id;
    std::string name;
    double score;
    task99::unpack(other, id, name, score);
    EXPECT_EQ(id, -1);
    EXPECT_EQ(name, "bob");
    EXPECT_DOUBLE_EQ(score, 0.0);
}

TEST(get_and_tie, getters_agree_with_unpack)
{
    int id;
    std::string name;
    double score;
    task99::unpack(kSample, id, name, score);
    EXPECT_EQ(id, task99::first_field(kSample));
    EXPECT_EQ(name, task99::second_field(kSample));
    EXPECT_DOUBLE_EQ(score, task99::third_field(kSample));
}
