#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task16;

TEST(order_of_evaluation, member_access_is_sequenced)
{
    EXPECT_TRUE(t::is_sequenced(t::scenario::member_access));
}

TEST(order_of_evaluation, arrow_call_is_sequenced)
{
    EXPECT_TRUE(t::is_sequenced(t::scenario::arrow_call));
}

TEST(order_of_evaluation, subscript_is_sequenced)
{
    EXPECT_TRUE(t::is_sequenced(t::scenario::subscript));
}

TEST(order_of_evaluation, stream_chain_is_sequenced)
{
    EXPECT_TRUE(t::is_sequenced(t::scenario::stream_chain));
}

TEST(order_of_evaluation, assignment_is_sequenced)
{
    EXPECT_TRUE(t::is_sequenced(t::scenario::assignment));
}

TEST(order_of_evaluation, function_arg_order_NOT_sequenced)
{
    EXPECT_FALSE(t::is_sequenced(t::scenario::function_arg_order));
}

TEST(order_of_evaluation, multiple_modifications_NOT_sequenced)
{
    EXPECT_FALSE(t::is_sequenced(t::scenario::multiple_modifications));
}
