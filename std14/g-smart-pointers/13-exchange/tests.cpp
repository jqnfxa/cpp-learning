#include <gtest/gtest.h>

#include <utility>

#include "solution.hpp"

namespace t = task13;

TEST(exchange, counter_pop_returns_old)
{
    t::counter c(42);
    EXPECT_EQ(c.pop(), 42);
}

TEST(exchange, counter_pop_resets_to_zero)
{
    /* The stub returns the value without resetting -- this is
     * the test that catches it. */
    t::counter c(42);
    c.pop();
    EXPECT_EQ(c.peek(), 0);
}

TEST(exchange, counter_pop_twice)
{
    t::counter c(99);
    EXPECT_EQ(c.pop(), 99);
    EXPECT_EQ(c.pop(), 0); /* second pop returns the reset value */
}

TEST(exchange, counter_swap_returns_old)
{
    t::counter c(7);
    EXPECT_EQ(c.swap(13), 7);
}

TEST(exchange, counter_swap_stores_new)
{
    t::counter c(7);
    c.swap(13);
    EXPECT_EQ(c.peek(), 13);
}

TEST(exchange, counter_swap_chain)
{
    t::counter c(1);
    int a = c.swap(2);
    int b = c.swap(3);
    int d = c.swap(4);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(d, 3);
    EXPECT_EQ(c.peek(), 4);
}

TEST(exchange, buffer_move_ctor_transfers_data)
{
    t::buffer src(10);
    int *src_data = src.data();
    t::buffer dst(std::move(src));
    EXPECT_EQ(dst.data(), src_data);
    EXPECT_EQ(dst.size(), 10u);
}

TEST(exchange, buffer_move_ctor_nulls_source_data)
{
    t::buffer src(10);
    t::buffer dst(std::move(src));
    EXPECT_EQ(src.data(), nullptr);
}

TEST(exchange, buffer_move_ctor_zeros_source_size)
{
    /* The KEY test for exchange usage: after move, the source's
     * size should be 0. The stub forgets to reset size_. */
    t::buffer src(10);
    t::buffer dst(std::move(src));
    EXPECT_EQ(src.size(), 0u);
}

TEST(exchange, buffer_move_assign_transfers)
{
    t::buffer src(20);
    int *src_data = src.data();
    t::buffer dst(5);
    dst = std::move(src);
    EXPECT_EQ(dst.data(), src_data);
    EXPECT_EQ(dst.size(), 20u);
}

TEST(exchange, buffer_move_assign_zeros_source)
{
    /* Same KEY test for move assignment. */
    t::buffer src(20);
    t::buffer dst(5);
    dst = std::move(src);
    EXPECT_EQ(src.data(), nullptr);
    EXPECT_EQ(src.size(), 0u);
}

TEST(exchange, buffer_destructor_safe_after_move)
{
    /* No double-delete. Move src into dst, both go out of
     * scope at end of block. Just having this test reach end-
     * of-scope without crash is the assertion. */
    {
        t::buffer src(50);
        t::buffer dst(std::move(src));
        (void)dst;
    }
    SUCCEED();
}
