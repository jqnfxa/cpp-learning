#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

TEST(all_any_none_of, is_all_positive_true)
{
    EXPECT_TRUE(task80::is_all_positive({1, 2, 3}));
    EXPECT_TRUE(task80::is_all_positive({42}));
}

TEST(all_any_none_of, is_all_positive_false_on_zero)
{
    EXPECT_FALSE(task80::is_all_positive({1, 0, 3}));
}

TEST(all_any_none_of, is_all_positive_false_on_negative)
{
    EXPECT_FALSE(task80::is_all_positive({1, -2, 3}));
}

TEST(all_any_none_of, is_all_positive_vacuously_true_on_empty)
{
    EXPECT_TRUE(task80::is_all_positive({}));
}

TEST(all_any_none_of, has_zero_true)
{
    EXPECT_TRUE(task80::has_zero({1, 0, -1}));
    EXPECT_TRUE(task80::has_zero({0}));
    EXPECT_TRUE(task80::has_zero({1, 2, 3, 0}));
}

TEST(all_any_none_of, has_zero_false_on_positives_and_negatives)
{
    EXPECT_FALSE(task80::has_zero({1, 2, 3}));
    EXPECT_FALSE(task80::has_zero({-1, -2, -3}));
}

TEST(all_any_none_of, has_zero_false_on_empty)
{
    EXPECT_FALSE(task80::has_zero({}));
}

TEST(all_any_none_of, none_negative_true)
{
    EXPECT_TRUE(task80::none_negative({0, 1, 2, 3}));
    EXPECT_TRUE(task80::none_negative({5}));
}

TEST(all_any_none_of, none_negative_false_on_single_negative)
{
    EXPECT_FALSE(task80::none_negative({1, 2, -3}));
}

TEST(all_any_none_of, none_negative_vacuously_true_on_empty)
{
    EXPECT_TRUE(task80::none_negative({}));
}

TEST(all_any_none_of, short_circuit_decisive_at_first_element)
{
    /* Constructed input where the decisive element is at position 0.
     * The algorithms should not have to walk the rest -- but we can't
     * observe that from a black-box test. We just verify the result. */
    std::vector<int> v(100000, 1);
    v[0] = -1;
    EXPECT_FALSE(task80::is_all_positive(v));
    EXPECT_FALSE(task80::none_negative(v));

    v[0] = 0;
    EXPECT_TRUE(task80::has_zero(v));
    EXPECT_FALSE(task80::is_all_positive(v));
    EXPECT_TRUE(task80::none_negative(v)); /* all >= 0 */
}
