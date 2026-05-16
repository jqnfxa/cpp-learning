#include <gtest/gtest.h>

#include <cstdint>

#include "solution.hpp"

/*
 * The enum is only forward-declared in solution.hpp, so tests.cpp
 * cannot reference task14::state::idle by name. All interactions go
 * through the factory + predicate API.
 *
 * sizeof and static_cast to the underlying type are still legal --
 * the underlying type is visible in the forward declaration.
 */

static_assert(sizeof(task14::state) == 1, "state has underlying type std::uint8_t (1 byte)");

TEST(forward_declared_enum, factories_return_distinct_values)
{
    auto idle = task14::make_idle();
    auto running = task14::make_running();
    auto done = task14::make_done();
    EXPECT_NE(idle, running);
    EXPECT_NE(running, done);
    EXPECT_NE(idle, done);
}

TEST(forward_declared_enum, factories_have_pinned_underlying_values)
{
    EXPECT_EQ(static_cast<std::uint8_t>(task14::make_idle()), 0);
    EXPECT_EQ(static_cast<std::uint8_t>(task14::make_running()), 1);
    EXPECT_EQ(static_cast<std::uint8_t>(task14::make_done()), 2);
}

TEST(forward_declared_enum, is_terminal_only_for_done)
{
    EXPECT_FALSE(task14::is_terminal(task14::make_idle()));
    EXPECT_FALSE(task14::is_terminal(task14::make_running()));
    EXPECT_TRUE(task14::is_terminal(task14::make_done()));
}

TEST(forward_declared_enum, name_of_idle)
{
    EXPECT_STREQ(task14::name_of(task14::make_idle()), "idle");
}

TEST(forward_declared_enum, name_of_running)
{
    EXPECT_STREQ(task14::name_of(task14::make_running()), "running");
}

TEST(forward_declared_enum, name_of_done)
{
    EXPECT_STREQ(task14::name_of(task14::make_done()), "done");
}
