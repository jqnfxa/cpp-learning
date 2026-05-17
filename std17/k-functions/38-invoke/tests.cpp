#include <gtest/gtest.h>

#include "solution.hpp"

TEST(Invoke, free_function)
{
    EXPECT_EQ(task38::call_free(5), 10);
}

TEST(Invoke, member_pointer)
{
    EXPECT_EQ(task38::call_member({42}), 42);
}

TEST(Invoke, lambda)
{
    EXPECT_EQ(task38::call_lambda(7), 8);
}
