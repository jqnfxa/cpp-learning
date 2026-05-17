#include <gtest/gtest.h>

#include <tuple>

#include "solution.hpp"

namespace t = task14;

TEST(integer_sequence, sum_three_ints)
{
    EXPECT_EQ(t::apply_sum(std::make_tuple(1, 2, 3)), 6);
}

TEST(integer_sequence, sum_single_int)
{
    EXPECT_EQ(t::apply_sum(std::make_tuple(42)), 42);
}

TEST(integer_sequence, sum_empty_tuple)
{
    /* Empty tuple: sum is 0. Matches the stub's hard-coded 0
     * -- this is a sanity test, not a stub-detector. */
    EXPECT_EQ(t::apply_sum(std::tuple<>{}), 0);
}

TEST(integer_sequence, sum_five_elements)
{
    EXPECT_EQ(t::apply_sum(std::make_tuple(10, 20, 30, 40, 50)), 150);
}

TEST(integer_sequence, sum_with_negatives)
{
    EXPECT_EQ(t::apply_sum(std::make_tuple(10, -3, 5, -2)), 10);
}

TEST(integer_sequence, product_three_ints)
{
    EXPECT_EQ(t::apply_product(std::make_tuple(2, 3, 4)), 24);
}

TEST(integer_sequence, product_with_zero)
{
    EXPECT_EQ(t::apply_product(std::make_tuple(7, 0, 13)), 0);
}

TEST(integer_sequence, product_single_element)
{
    EXPECT_EQ(t::apply_product(std::make_tuple(42)), 42);
}

TEST(integer_sequence, product_empty_tuple)
{
    /* Empty tuple: product is 1 (empty product). Matches stub.
     * Sanity test. */
    EXPECT_EQ(t::apply_product(std::tuple<>{}), 1);
}

TEST(integer_sequence, sum_first_three_of_five)
{
    /* sum_first_n uses std::make_index_sequence<N> directly. */
    EXPECT_EQ((t::sum_first_n<3>(std::make_tuple(10, 20, 30, 40, 50))), 60);
}

TEST(integer_sequence, sum_first_one)
{
    EXPECT_EQ((t::sum_first_n<1>(std::make_tuple(10, 20, 30, 40, 50))), 10);
}

TEST(integer_sequence, sum_first_zero)
{
    /* Empty prefix. Matches stub. Sanity check. */
    EXPECT_EQ((t::sum_first_n<0>(std::make_tuple(10, 20, 30, 40, 50))), 0);
}

TEST(integer_sequence, sum_first_all_five)
{
    EXPECT_EQ((t::sum_first_n<5>(std::make_tuple(10, 20, 30, 40, 50))), 150);
}
