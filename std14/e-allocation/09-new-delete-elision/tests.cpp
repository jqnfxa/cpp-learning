#include <gtest/gtest.h>

#include "solution.hpp"

namespace t = task09;

TEST(new_delete_elision, local_pair_is_elidable)
{
    EXPECT_TRUE(t::may_elide(t::scenario::local_new_delete_paired));
}

TEST(new_delete_elision, escape_to_unknown_callee_is_not_elidable)
{
    EXPECT_FALSE(t::may_elide(t::scenario::pointer_escapes_to_unknown_callee));
}

TEST(new_delete_elision, make_shared_fused_is_elidable)
{
    /* std::make_shared is the standard's canonical fusion
     * example: one `operator new` for control block + T. */
    EXPECT_TRUE(t::may_elide(t::scenario::make_shared_fused_allocation));
}

TEST(new_delete_elision, replaced_operator_new_blocks_elision)
{
    /* A user-supplied ::operator new has observable side
     * effects; eliding would change them. */
    EXPECT_FALSE(t::may_elide(t::scenario::replaced_global_operator_new));
}

TEST(new_delete_elision, new_without_visible_delete_is_not_elidable)
{
    EXPECT_FALSE(
        t::may_elide(t::scenario::new_without_matching_delete_in_scope));
}

TEST(new_delete_elision, function_is_constexpr)
{
    /* The classifier is pure -- it just maps scenario -> bool.
     * Should be usable in a constant expression. */
    constexpr bool ok = t::may_elide(t::scenario::local_new_delete_paired);
    /* On the reference: ok == true. On the stub: ok == false. */
    EXPECT_TRUE(ok);
}
