#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task14;

class InlineVarTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        t::reset_counter();
    }
};

TEST_F(InlineVarTest, initial_counter_is_zero)
{
    EXPECT_EQ(t::read_counter(), 0);
}

TEST_F(InlineVarTest, increment_returns_one)
{
    EXPECT_EQ(t::increment_counter(), 1);
}

TEST_F(InlineVarTest, increment_increases_counter)
{
    t::increment_counter();
    t::increment_counter();
    t::increment_counter();
    EXPECT_EQ(t::read_counter(), 3);
}

TEST_F(InlineVarTest, max_retries_is_3)
{
    EXPECT_EQ(t::max_retries, 3);
}

TEST_F(InlineVarTest, pi_approx_value)
{
    EXPECT_NEAR(t::pi_approx, 3.14159, 1e-5);
}

TEST_F(InlineVarTest, helper_sees_same_counter)
{
    /* The KEY test: tests.cpp and helper.cpp must see the
     * SAME global_counter. With C++17 `inline`, they share
     * one definition (passes). With `static`, each TU has
     * its own copy (helper sees 0, tests.cpp sees 1, test
     * fails). */
    t::increment_counter(); /* tests.cpp's counter: 1 */
    int helper_view = t::helper_read_counter();
    EXPECT_EQ(helper_view, 1);
}

TEST_F(InlineVarTest, cross_tu_increment_chain)
{
    /* Increment from one TU, read from the other -- should
     * advance the same counter. */
    int a = t::increment_counter();        /* tests.cpp: 1 */
    int b = t::helper_increment_counter(); /* helper.cpp: 2 if shared */
    int c = t::increment_counter();        /* tests.cpp: 3 if shared */
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
}
