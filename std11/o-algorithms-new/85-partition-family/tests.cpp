#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "solution.hpp"

/* ----- is_partitioned_by_sign -------------------------------------- */

TEST(partition_family, is_partitioned_true_for_partitioned_input)
{
    EXPECT_TRUE(task85::is_partitioned_by_sign({-3, -1, 0, 5, 8}));
    EXPECT_TRUE(task85::is_partitioned_by_sign({-1, 0}));
}

TEST(partition_family, is_partitioned_false_for_unpartitioned_input)
{
    EXPECT_FALSE(task85::is_partitioned_by_sign({-1, 0, -2}));
    EXPECT_FALSE(task85::is_partitioned_by_sign({1, -1}));
}

TEST(partition_family, is_partitioned_empty_is_true)
{
    EXPECT_TRUE(task85::is_partitioned_by_sign({}));
}

TEST(partition_family, is_partitioned_all_negatives)
{
    EXPECT_TRUE(task85::is_partitioned_by_sign({-5, -3, -1}));
}

TEST(partition_family, is_partitioned_all_nonnegatives)
{
    EXPECT_TRUE(task85::is_partitioned_by_sign({0, 1, 2, 3}));
}

/* ----- split_by_sign ----------------------------------------------- */

TEST(partition_family, split_basic)
{
    auto split = task85::split_by_sign({3, -1, 4, -1, 5, 9, -2, 6});
    EXPECT_EQ(split.first, (std::vector<int>{-1, -1, -2}));
    EXPECT_EQ(split.second, (std::vector<int>{3, 4, 5, 9, 6}));
}

TEST(partition_family, split_preserves_order_within_each_bucket)
{
    auto split = task85::split_by_sign({-1, 2, -3, 4, -5, 6});
    EXPECT_EQ(split.first, (std::vector<int>{-1, -3, -5}));
    EXPECT_EQ(split.second, (std::vector<int>{2, 4, 6}));
}

TEST(partition_family, split_empty_input)
{
    auto split = task85::split_by_sign({});
    EXPECT_TRUE(split.first.empty());
    EXPECT_TRUE(split.second.empty());
}

TEST(partition_family, split_zero_is_non_negative)
{
    auto split = task85::split_by_sign({-1, 0, 1});
    EXPECT_EQ(split.first, std::vector<int>{-1});
    EXPECT_EQ(split.second, (std::vector<int>{0, 1}));
}

/* ----- partition_point_by_sign ------------------------------------ */

TEST(partition_family, partition_point_mixed)
{
    /* {-3, -1, 0, 5, 8} -- first non-negative is at index 2. */
    EXPECT_EQ(task85::partition_point_by_sign({-3, -1, 0, 5, 8}), 2u);
}

TEST(partition_family, partition_point_all_negatives_returns_size)
{
    EXPECT_EQ(task85::partition_point_by_sign({-3, -2, -1}), 3u);
}

TEST(partition_family, partition_point_all_nonnegatives_returns_zero)
{
    EXPECT_EQ(task85::partition_point_by_sign({0, 1, 2}), 0u);
}

TEST(partition_family, partition_point_empty_returns_zero)
{
    EXPECT_EQ(task85::partition_point_by_sign({}), 0u);
}

TEST(partition_family, partition_point_single_negative)
{
    EXPECT_EQ(task85::partition_point_by_sign({-1}), 1u);
}

TEST(partition_family, partition_point_single_nonnegative)
{
    EXPECT_EQ(task85::partition_point_by_sign({7}), 0u);
}
