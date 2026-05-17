#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task23;

TEST(auto_ptr_replacement, make_widget_not_null)
{
    auto w = t::make_widget(42);
    ASSERT_NE(w, nullptr);
    EXPECT_EQ(w->value, 42);
}

TEST(auto_ptr_replacement, move_and_read)
{
    auto w = t::make_widget(7);
    EXPECT_EQ(t::move_and_read(std::move(w)), 7);
}

TEST(auto_ptr_replacement, source_null_after_move)
{
    auto w = t::make_widget(10);
    t::move_and_read(std::move(w));
    EXPECT_EQ(w, nullptr);
}
