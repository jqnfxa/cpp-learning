#include <gtest/gtest.h>

#include <deque>
#include <string>
#include <vector>

#include "solution.hpp"

namespace
{

class emplace_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        task77::reset_counters();
    }
};

}

TEST_F(emplace_test, push_back_does_value_plus_move)
{
    auto v = task77::via_push_back(42, "alpha");
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0].n, 42);
    EXPECT_EQ(v[0].name, "alpha");
    EXPECT_EQ(task77::value_ctor_count(), 1) << "exactly one value ctor (the temporary)";
    EXPECT_EQ(task77::move_ctor_count(), 1) << "the temporary is moved into the vector";
    EXPECT_EQ(task77::copy_ctor_count(), 0) << "Tracker is movable, no copy needed";
}

TEST_F(emplace_test, emplace_back_does_value_only)
{
    auto v = task77::via_emplace_back(7, "beta");
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0].n, 7);
    EXPECT_EQ(v[0].name, "beta");
    EXPECT_EQ(task77::value_ctor_count(), 1) << "Tracker is constructed in place";
    EXPECT_EQ(task77::move_ctor_count(), 0) << "no temporary -> no move";
    EXPECT_EQ(task77::copy_ctor_count(), 0);
}

TEST_F(emplace_test, emplace_front_does_value_only_on_deque)
{
    auto d = task77::via_emplace_front(9, "gamma");
    ASSERT_EQ(d.size(), 1u);
    EXPECT_EQ(d.front().n, 9);
    EXPECT_EQ(d.front().name, "gamma");
    EXPECT_EQ(task77::value_ctor_count(), 1);
    EXPECT_EQ(task77::move_ctor_count(), 0);
    EXPECT_EQ(task77::copy_ctor_count(), 0);
}

TEST_F(emplace_test, push_back_and_emplace_back_produce_same_logical_value)
{
    auto a = task77::via_push_back(5, "same");
    task77::reset_counters();
    auto b = task77::via_emplace_back(5, "same");

    ASSERT_EQ(a.size(), 1u);
    ASSERT_EQ(b.size(), 1u);
    EXPECT_EQ(a[0].n, b[0].n);
    EXPECT_EQ(a[0].name, b[0].name);
}

TEST_F(emplace_test, emplace_back_saves_one_move_per_call)
{
    /* Quantitative proof: push_back uses 1 value + 1 move = 2
     * constructions; emplace_back uses 1 value + 0 moves = 1
     * construction. emplace_back wins by exactly one move per
     * call. */
    task77::reset_counters();
    (void)task77::via_push_back(1, "x");
    int pb_total = task77::value_ctor_count() + task77::move_ctor_count();

    task77::reset_counters();
    (void)task77::via_emplace_back(1, "x");
    int eb_total = task77::value_ctor_count() + task77::move_ctor_count();

    EXPECT_EQ(pb_total, 2);
    EXPECT_EQ(eb_total, 1);
    EXPECT_EQ(pb_total - eb_total, 1);
}
