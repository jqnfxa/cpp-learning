#include <gtest/gtest.h>

#include "solution.hpp"

TEST(MakeFromTuple, basic)
{
    auto p = task35::make_point({3, 4});
    EXPECT_EQ(p.x, 3);
    EXPECT_EQ(p.y, 4);
}

TEST(MakeFromTuple, negatives)
{
    auto p = task35::make_point({-7, 99});
    EXPECT_EQ(p.x, -7);
    EXPECT_EQ(p.y, 99);
}
