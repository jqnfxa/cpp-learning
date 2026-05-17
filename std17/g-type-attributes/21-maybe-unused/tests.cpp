#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task21;

TEST(maybe_unused, do_nothing_returns_42)
{
    EXPECT_EQ(t::do_nothing(99), 42);
}

TEST(maybe_unused, complex_op_multiplies)
{
    EXPECT_EQ(t::complex_op(3, 7), 21);
}

TEST(maybe_unused, complex_op_zero)
{
    EXPECT_EQ(t::complex_op(0, 5), 0);
}
