#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

TEST(next_prev, nth_element_basic)
{
    std::vector<int> v{10, 20, 30, 40, 50};
    EXPECT_EQ(task93::nth_element(v, 0), 10);
    EXPECT_EQ(task93::nth_element(v, 1), 20);
    EXPECT_EQ(task93::nth_element(v, 2), 30);
    EXPECT_EQ(task93::nth_element(v, 3), 40);
    EXPECT_EQ(task93::nth_element(v, 4), 50);
}

TEST(next_prev, nth_element_single)
{
    std::vector<int> v{42};
    EXPECT_EQ(task93::nth_element(v, 0), 42);
}

TEST(next_prev, nth_from_end_basic)
{
    std::vector<int> v{10, 20, 30, 40, 50};
    EXPECT_EQ(task93::nth_from_end(v, 1), 50); /* last */
    EXPECT_EQ(task93::nth_from_end(v, 2), 40);
    EXPECT_EQ(task93::nth_from_end(v, 3), 30);
    EXPECT_EQ(task93::nth_from_end(v, 4), 20);
    EXPECT_EQ(task93::nth_from_end(v, 5), 10);
}

TEST(next_prev, nth_from_end_single)
{
    std::vector<int> v{42};
    EXPECT_EQ(task93::nth_from_end(v, 1), 42);
}

TEST(next_prev, both_agree_on_same_position)
{
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7};
    /* v[k] == nth_element(v, k) == nth_from_end(v, size - k). */
    for (std::size_t k = 0; k < v.size(); ++k)
    {
        EXPECT_EQ(task93::nth_element(v, k),
                  task93::nth_from_end(v, v.size() - k))
            << "k=" << k;
    }
}

TEST(next_prev, negatives)
{
    std::vector<int> v{-3, -2, -1, 0, 1};
    EXPECT_EQ(task93::nth_element(v, 0), -3);
    EXPECT_EQ(task93::nth_element(v, 4), 1);
    EXPECT_EQ(task93::nth_from_end(v, 1), 1);
    EXPECT_EQ(task93::nth_from_end(v, 5), -3);
}

TEST(next_prev, large_index)
{
    std::vector<int> v(1000);
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        v[i] = static_cast<int>(i);
    }
    EXPECT_EQ(task93::nth_element(v, 999), 999);
    EXPECT_EQ(task93::nth_from_end(v, 1), 999);
    EXPECT_EQ(task93::nth_element(v, 500), 500);
    EXPECT_EQ(task93::nth_from_end(v, 500), 500);
}
