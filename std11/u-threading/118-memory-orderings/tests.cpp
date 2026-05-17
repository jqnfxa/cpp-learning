#include <gtest/gtest.h>

#include "solution.hpp"

TEST(memory_orderings, handoff_basic)
{
    EXPECT_EQ(task118::handoff(42), 42);
}

TEST(memory_orderings, handoff_zero)
{
    EXPECT_EQ(task118::handoff(0), 0);
}

TEST(memory_orderings, handoff_negative)
{
    EXPECT_EQ(task118::handoff(-1), -1);
    EXPECT_EQ(task118::handoff(-100000), -100000);
}

TEST(memory_orderings, handoff_large_positive)
{
    EXPECT_EQ(task118::handoff(1000000), 1000000);
    EXPECT_EQ(task118::handoff(2000000000), 2000000000);
}

TEST(memory_orderings, repeated_handoffs)
{
    /* Stress: 1000 handoffs. With proper release/acquire, every one
     * delivers the right payload. With the stub (returns 0), all
     * non-zero payloads fail. */
    for (int i = 1; i <= 1000; ++i)
    {
        EXPECT_EQ(task118::handoff(i), i) << "i=" << i;
    }
}

TEST(memory_orderings, handoff_with_int_min_and_max)
{
    /* Edge values of int.  The payload is an int parameter; the
     * handoff must preserve it bit-for-bit. */
    EXPECT_EQ(task118::handoff(2147483647), 2147483647);     /* INT_MAX */
    EXPECT_EQ(task118::handoff(-2147483648LL), -2147483648); /* INT_MIN */
}
