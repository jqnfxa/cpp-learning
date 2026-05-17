#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task05;

TEST(copy_elision, lock_handle_compiles_and_returns)
{
    /* If this compiles at all, guaranteed copy elision is
     * in effect -- lock_handle is non-movable, but the
     * factory returns it by value. */
    t::lock_handle h = t::make_lock(42);
    EXPECT_EQ(h.id, 42);
}

TEST(copy_elision, lock_handle_zero_id)
{
    t::lock_handle h = t::make_lock(0);
    EXPECT_EQ(h.id, 0);
}

TEST(copy_elision, lock_handle_negative_id)
{
    /* This catches the stub's hardcoded -1. */
    t::lock_handle h = t::make_lock(123);
    EXPECT_EQ(h.id, 123);
}

TEST(copy_elision, lock_handle_large_id)
{
    t::lock_handle h = t::make_lock(1000000);
    EXPECT_EQ(h.id, 1000000);
}

TEST(copy_elision, make_traced_no_moves)
{
    /* The KEY test: returning a PRVALUE means zero moves and
     * zero copies. With the stub's std::move(t), at least
     * one move is recorded. */
    t::trace_type::reset();
    t::trace_type x = t::make_traced(99);
    EXPECT_EQ(x.value, 99);
    EXPECT_EQ(t::trace_type::move_count(), 0);
    EXPECT_EQ(t::trace_type::copy_count(), 0);
}

TEST(copy_elision, make_traced_value_correct)
{
    /* Value must be right regardless of move count -- both
     * stub and reference store v correctly; this is a sanity
     * test. */
    t::trace_type::reset();
    auto x = t::make_traced(7);
    EXPECT_EQ(x.value, 7);
}

TEST(copy_elision, make_traced_chained_factory_calls)
{
    /* Multiple calls in a row -- each one must not move. */
    t::trace_type::reset();
    auto a = t::make_traced(1);
    auto b = t::make_traced(2);
    auto c = t::make_traced(3);
    EXPECT_EQ(a.value, 1);
    EXPECT_EQ(b.value, 2);
    EXPECT_EQ(c.value, 3);
    EXPECT_EQ(t::trace_type::move_count(), 0);
    EXPECT_EQ(t::trace_type::copy_count(), 0);
}

TEST(copy_elision, explicit_move_does_count)
{
    /* Sanity: explicit std::move DOES register a move (to
     * prove the trace_type counter works). */
    t::trace_type::reset();
    t::trace_type x(10);
    t::trace_type y = std::move(x);
    EXPECT_EQ(y.value, 10);
    EXPECT_EQ(t::trace_type::move_count(), 1);
}
