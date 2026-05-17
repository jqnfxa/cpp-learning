#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task06;

TEST(temporary_materialization, count_zero_with_extension)
{
    /* The KEY test: when the prvalue is bound to a const-ref,
     * destruction is DELAYED until the ref's scope ends. So
     * the count inside the scope is 0. The stub doesn't bind,
     * so the temp dies at the semicolon and count is 1. */
    EXPECT_EQ(t::destruction_count_within_scope(), 0);
}

TEST(temporary_materialization, peek_id_42)
{
    EXPECT_EQ(t::peek_through_ref(), 42);
}

TEST(temporary_materialization, manual_const_ref_extension)
{
    /* The textbook usage: const T& bound to a prvalue extends
     * the prvalue's lifetime to the reference's scope. */
    t::trace::reset();
    int id_during;
    int destroyed_after;
    {
        const t::trace &x = t::trace(7);
        id_during = x.id;
    }
    destroyed_after = t::trace::destroyed_count();
    EXPECT_EQ(id_during, 7);
    EXPECT_EQ(destroyed_after, 1);
}

TEST(temporary_materialization, multiple_temporaries_extended)
{
    t::trace::reset();
    {
        const t::trace &a = t::trace(1);
        const t::trace &b = t::trace(2);
        const t::trace &c = t::trace(3);
        EXPECT_EQ(a.id, 1);
        EXPECT_EQ(b.id, 2);
        EXPECT_EQ(c.id, 3);
        EXPECT_EQ(t::trace::destroyed_count(), 0); /* all alive */
    }
    EXPECT_EQ(t::trace::destroyed_count(), 3); /* all gone */
}

TEST(temporary_materialization, count_increments_on_each_destruction)
{
    /* Sanity: the trace's destructor counter works. */
    t::trace::reset();
    {
        const t::trace &a = t::trace(1);
        (void)a;
    }
    EXPECT_EQ(t::trace::destroyed_count(), 1);
}

TEST(temporary_materialization, rvalue_ref_also_extends)
{
    /* T&& binding to a prvalue ALSO extends -- same rule.
     * Used to be a discriminator for "lvalue ref vs rvalue
     * ref" before C++11, but in C++11+ rvalue refs use the
     * same lifetime-extension rule. */
    t::trace::reset();
    {
        t::trace &&r = t::trace(99);
        EXPECT_EQ(r.id, 99);
        EXPECT_EQ(t::trace::destroyed_count(), 0);
    }
    EXPECT_EQ(t::trace::destroyed_count(), 1);
}
