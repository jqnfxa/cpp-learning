#include <gtest/gtest.h>

#include "solution.hpp"

TEST(nullptr_overload, integer_zero_picks_int_overload)
{
    EXPECT_EQ(task01::f(0), 1);
}

TEST(nullptr_overload, nonzero_integer_picks_int_overload)
{
    EXPECT_EQ(task01::f(42), 1);
}

TEST(nullptr_overload, nullptr_literal_picks_pointer_overload)
{
    EXPECT_EQ(task01::f(nullptr), 2);
}

TEST(nullptr_overload, address_of_local_picks_pointer_overload)
{
    int x = 5;
    EXPECT_EQ(task01::f(&x), 2);
}
