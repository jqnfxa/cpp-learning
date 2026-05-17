#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "solution.hpp"

namespace t = task06;

TEST(init_capture, counter_starts_at_initial)
{
    auto c = t::make_counter(10);
    EXPECT_EQ(c(), 10);
}

TEST(init_capture, counter_negative_initial)
{
    auto c = t::make_counter(-5);
    EXPECT_EQ(c(), -5);
}

TEST(init_capture, counter_increments_across_calls)
{
    auto c = t::make_counter(7);
    EXPECT_EQ(c(), 7);
    EXPECT_EQ(c(), 8);
    EXPECT_EQ(c(), 9);
}

TEST(init_capture, counter_zero_initial_works)
{
    /* Sanity: 0 was the stub's hard-coded value. The reference
     * still has to honour an explicit 0 from the caller. */
    auto c = t::make_counter(0);
    EXPECT_EQ(c(), 0);
    EXPECT_EQ(c(), 1);
}

TEST(init_capture, multiple_counters_are_independent)
{
    auto a = t::make_counter(10);
    auto b = t::make_counter(100);
    EXPECT_EQ(a(), 10);
    EXPECT_EQ(b(), 100);
    EXPECT_EQ(a(), 11);
    EXPECT_EQ(b(), 101);
}

TEST(init_capture, growable_appends_to_initial)
{
    auto g = t::make_growable("abc");
    /* "abc" + "!" -> size 4 */
    EXPECT_EQ(g(), 4u);
}

TEST(init_capture, growable_starts_from_correct_string)
{
    auto g = t::make_growable("hello");
    /* "hello!" -> 6 */
    EXPECT_EQ(g(), 6u);
}

TEST(init_capture, growable_grows_with_each_call)
{
    auto g = t::make_growable("x");
    EXPECT_EQ(g(), 2u); /* "x!"   */
    EXPECT_EQ(g(), 3u); /* "x!!"  */
    EXPECT_EQ(g(), 4u); /* "x!!!" */
}

TEST(init_capture, growable_empty_initial)
{
    /* The stub hard-codes an empty initial string -- so for an
     * explicit empty input, stub and reference happen to agree.
     * Still a useful sanity check. */
    auto g = t::make_growable("");
    EXPECT_EQ(g(), 1u);
}

TEST(init_capture, growable_longer_initial)
{
    auto g = t::make_growable("0123456789");
    EXPECT_EQ(g(), 11u);
}

TEST(init_capture, hold_unique_returns_pointed_value)
{
    auto f = t::hold_unique(std::make_unique<int>(42));
    EXPECT_EQ(f(), 42);
}

TEST(init_capture, hold_unique_zero)
{
    auto f = t::hold_unique(std::make_unique<int>(0));
    EXPECT_EQ(f(), 0);
}

TEST(init_capture, hold_unique_negative)
{
    auto f = t::hold_unique(std::make_unique<int>(-100));
    EXPECT_EQ(f(), -100);
}

TEST(init_capture, hold_unique_callable_repeatedly)
{
    auto f = t::hold_unique(std::make_unique<int>(7));
    EXPECT_EQ(f(), 7);
    EXPECT_EQ(f(), 7);
    EXPECT_EQ(f(), 7);
}
