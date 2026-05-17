#include <gtest/gtest.h>

#include <memory>

#include "solution.hpp"

namespace
{

class weak_ptr_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        /* If the previous test leaked due to a cycle, the counters
         * won't be back at zero. We *don't* reset them here -- that
         * would mask the leak. Instead, abort the test now so it's
         * obvious which prior test was the offender. */
        ASSERT_EQ(task71::parents_alive(), 0) << "leak from a prior test";
        ASSERT_EQ(task71::children_alive(), 0) << "leak from a prior test";
    }
};

}

TEST_F(weak_ptr_test, link_makes_parent_own_child)
{
    auto p = task71::make_parent("dad");
    auto c = task71::make_child("kid");

    task71::link(p, c);

    ASSERT_EQ(p->children.size(), 1u);
    EXPECT_EQ(p->children[0].get(), c.get());

    /* `c` is held by the test scope AND by p->children[0] -> count == 2. */
    EXPECT_EQ(c.use_count(), 2);
}

TEST_F(weak_ptr_test, parent_use_count_is_one_when_back_ref_is_weak)
{
    auto p = task71::make_parent("dad");
    auto c = task71::make_child("kid");
    task71::link(p, c);

    /* If Child::parent is a weak_ptr, parent's strong count stays at
     * 1. If it's still a shared_ptr (the stub), strong count is 2 --
     * which is exactly the half of the cycle that needs fixing. */
    EXPECT_EQ(p.use_count(), 1);
}

TEST_F(weak_ptr_test, parent_name_returns_name_while_parent_alive)
{
    auto p = task71::make_parent("dad");
    auto c = task71::make_child("kid");
    task71::link(p, c);

    EXPECT_EQ(c->parent_name(), "dad");
}

TEST_F(weak_ptr_test, parent_name_returns_empty_after_parent_dies)
{
    auto c = task71::make_child("kid");
    {
        auto p = task71::make_parent("dad");
        task71::link(p, c);
        EXPECT_EQ(c->parent_name(), "dad");
    }
    /* External `p` is gone. If Child::parent is a weak_ptr, the only
     * strong owner of Parent was `p` -> Parent destroyed -> the weak
     * ref is now expired -> parent_name returns "".
     *
     * If Child::parent is still shared_ptr, the parent is kept alive
     * by `c->parent` and the test fails (returns "dad"). */
    EXPECT_EQ(c->parent_name(), "");
}

TEST_F(weak_ptr_test, dropping_both_handles_collects_both_objects)
{
    {
        auto p = task71::make_parent("dad");
        auto c = task71::make_child("kid");
        task71::link(p, c);
        /* Inside the scope: both alive. */
        EXPECT_EQ(task71::parents_alive(), 1);
        EXPECT_EQ(task71::children_alive(), 1);
    }
    /* Both `p` and `c` dropped. With the cycle fixed (weak back-ref):
     *   p drops -> Parent's count goes 1 -> 0 -> Parent destructed
     *   -> Parent's children vector destructed
     *   -> shared_ptr<Child> in that vector goes 2 -> 1 (only `c` was
     *      the other owner, but `c` is also gone now, so really
     *      we count: external c dropped first or after, either way
     *      both refs are gone) -> Child destructed.
     *
     * With the stub (shared back-ref), Parent keeps Child alive AND
     * Child keeps Parent alive -- classic cycle. Both leak. */
    EXPECT_EQ(task71::parents_alive(), 0);
    EXPECT_EQ(task71::children_alive(), 0);
}

TEST_F(weak_ptr_test, child_outlives_parent_when_held_externally)
{
    auto c = task71::make_child("kid");
    {
        auto p = task71::make_parent("dad");
        task71::link(p, c);
    }
    /* `p` is gone. `c` still held externally. */
    EXPECT_EQ(task71::parents_alive(), 0);
    EXPECT_EQ(task71::children_alive(), 1);
    EXPECT_EQ(c->parent_name(), "");
}
