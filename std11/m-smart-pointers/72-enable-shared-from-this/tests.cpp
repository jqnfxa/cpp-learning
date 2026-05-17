#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "solution.hpp"

namespace
{

class enable_shared_from_this_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        /* Each ctest test runs in its own process, but assert here
         * anyway to make any leak self-evident. */
        ASSERT_EQ(task72::widgets_alive(), 0) << "leak from prior test";
    }
};

}

TEST_F(enable_shared_from_this_test, make_widget_gives_owning_pointer)
{
    auto w = task72::make_widget(42);
    ASSERT_NE(w.get(), nullptr);
    EXPECT_EQ(w->value(), 42);
    EXPECT_EQ(w.use_count(), 1);
}

TEST_F(enable_shared_from_this_test, self_shares_control_block)
{
    auto w = task72::make_widget(7);
    auto same = w->self();
    ASSERT_NE(same.get(), nullptr);
    EXPECT_EQ(same.get(), w.get());
    /* Two shared_ptrs to the same object via the *same* control block. */
    EXPECT_EQ(w.use_count(), 2);
    EXPECT_EQ(same.use_count(), 2);
}

TEST_F(enable_shared_from_this_test, self_does_not_double_free)
{
    {
        auto w = task72::make_widget(7);
        auto a = w->self();
        auto b = w->self();
        auto c = a->self();
        EXPECT_EQ(w.use_count(), 4);
    }
    /* If self() handed out a second control block, the destructor
     * would run twice and corrupt this counter (or crash). The fact
     * that it returns cleanly and lands at 0 is the proof. */
    EXPECT_EQ(task72::widgets_alive(), 0);
}

TEST_F(enable_shared_from_this_test, subscribe_pushes_into_roster)
{
    std::vector<std::shared_ptr<task72::Widget>> roster;
    auto w = task72::make_widget(99);

    w->subscribe(roster);

    ASSERT_EQ(roster.size(), 1u);
    EXPECT_EQ(roster[0].get(), w.get());
    /* w + roster[0] -> use_count == 2. */
    EXPECT_EQ(w.use_count(), 2);
}

TEST_F(enable_shared_from_this_test, roster_alone_keeps_widget_alive)
{
    std::vector<std::shared_ptr<task72::Widget>> roster;
    {
        auto w = task72::make_widget(99);
        w->subscribe(roster);
    } /* `w` dropped, roster still holds */

    EXPECT_EQ(task72::widgets_alive(), 1);
    ASSERT_EQ(roster.size(), 1u);
    EXPECT_EQ(roster[0]->value(), 99);

    roster.clear();
    EXPECT_EQ(task72::widgets_alive(), 0);
}

TEST_F(enable_shared_from_this_test, subscribe_multiple_widgets)
{
    std::vector<std::shared_ptr<task72::Widget>> roster;
    auto a = task72::make_widget(1);
    auto b = task72::make_widget(2);
    auto c = task72::make_widget(3);

    a->subscribe(roster);
    b->subscribe(roster);
    c->subscribe(roster);

    ASSERT_EQ(roster.size(), 3u);
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(b.use_count(), 2);
    EXPECT_EQ(c.use_count(), 2);
    /* All three widgets distinct -- self() didn't mash them together. */
    EXPECT_NE(roster[0].get(), roster[1].get());
    EXPECT_NE(roster[1].get(), roster[2].get());
}
