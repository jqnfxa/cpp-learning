#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

namespace t = task09;

TEST(fold_expressions, sum_two_ints)
{
    EXPECT_EQ(t::sum(1, 2), 3);
}

TEST(fold_expressions, sum_three_ints)
{
    EXPECT_EQ(t::sum(1, 2, 3), 6);
}

TEST(fold_expressions, sum_five_ints)
{
    EXPECT_EQ(t::sum(10, 20, 30, 40, 50), 150);
}

TEST(fold_expressions, sum_mixed_types)
{
    EXPECT_DOUBLE_EQ(t::sum(1, 2.5, 3), 6.5);
}

TEST(fold_expressions, product_two_ints)
{
    EXPECT_EQ(t::product(3, 4), 12);
}

TEST(fold_expressions, product_with_zero)
{
    EXPECT_EQ(t::product(1, 2, 0, 4), 0);
}

TEST(fold_expressions, product_three_ints)
{
    EXPECT_EQ(t::product(2, 3, 5), 30);
}

TEST(fold_expressions, all_true_all_true)
{
    EXPECT_TRUE(t::all_true(true, true, true));
}

TEST(fold_expressions, all_true_one_false)
{
    EXPECT_FALSE(t::all_true(true, false, true));
}

TEST(fold_expressions, all_true_predicates)
{
    EXPECT_TRUE(t::all_true(1 > 0, 2 > 1, 3 > 2));
    EXPECT_FALSE(t::all_true(1 > 0, 2 > 3, 3 > 2));
}

TEST(fold_expressions, concat_one_int)
{
    EXPECT_EQ(t::concat_with_spaces(42), "42 ");
}

TEST(fold_expressions, concat_three_mixed)
{
    EXPECT_EQ(t::concat_with_spaces(1, std::string("hi"), 2.5),
              "1 hi 2.5 ");
}

TEST(fold_expressions, concat_two_strings)
{
    EXPECT_EQ(t::concat_with_spaces(std::string("alpha"),
                                    std::string("beta")),
              "alpha beta ");
}
