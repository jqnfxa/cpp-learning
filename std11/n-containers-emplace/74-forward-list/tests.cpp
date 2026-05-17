#include <gtest/gtest.h>

#include <forward_list>
#include <vector>

#include "solution.hpp"

TEST(forward_list, length_empty)
{
    task74::list l;
    EXPECT_EQ(task74::length(l), 0u);
}

TEST(forward_list, length_nonempty)
{
    task74::list l{1, 2, 3, 4, 5};
    EXPECT_EQ(task74::length(l), 5u);
}

TEST(forward_list, to_vector_round_trip)
{
    task74::list l{10, 20, 30};
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{10, 20, 30}));
}

TEST(forward_list, insert_at_head_empty_list)
{
    task74::list l;
    task74::insert_at(l, 0, 7);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{7}));
}

TEST(forward_list, insert_at_head_nonempty)
{
    task74::list l{2, 3, 4};
    task74::insert_at(l, 0, 1);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{1, 2, 3, 4}));
}

TEST(forward_list, insert_at_middle)
{
    task74::list l{1, 2, 4, 5};
    task74::insert_at(l, 2, 3);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(forward_list, insert_at_end)
{
    task74::list l{1, 2, 3};
    task74::insert_at(l, 3, 4); /* index == length -> append */
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{1, 2, 3, 4}));
}

TEST(forward_list, erase_at_head)
{
    task74::list l{1, 2, 3};
    task74::erase_at(l, 0);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{2, 3}));
}

TEST(forward_list, erase_at_middle)
{
    task74::list l{1, 2, 3, 4};
    task74::erase_at(l, 1);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{1, 3, 4}));
}

TEST(forward_list, erase_at_last)
{
    task74::list l{1, 2, 3};
    task74::erase_at(l, 2);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{1, 2}));
}

TEST(forward_list, insert_then_erase_round_trip)
{
    task74::list l{1, 2, 4, 5};
    task74::insert_at(l, 2, 3);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{1, 2, 3, 4, 5}));
    task74::erase_at(l, 2);
    EXPECT_EQ(task74::to_vector(l), (std::vector<int>{1, 2, 4, 5}));
}

TEST(forward_list, length_tracks_after_mutations)
{
    task74::list l{1, 2, 3};
    EXPECT_EQ(task74::length(l), 3u);
    task74::insert_at(l, 0, 0);
    EXPECT_EQ(task74::length(l), 4u);
    task74::erase_at(l, 2);
    EXPECT_EQ(task74::length(l), 3u);
}
