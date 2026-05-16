#include <gtest/gtest.h>

#include "solution.hpp"

TEST(final_virtual, base_rate_returns_1)
{
    task27::base b{};
    EXPECT_EQ(b.rate(), 1);
}

TEST(final_virtual, middle_rate_returns_2)
{
    task27::middle m{};
    EXPECT_EQ(m.rate(), 2);
}

TEST(final_virtual, leaf_inherits_middle_rate)
{
    task27::leaf l{};
    /* leaf can't override rate (middle marked it final), so the
     * call resolves to middle::rate -> 2. */
    EXPECT_EQ(l.rate(), 2);
}

TEST(final_virtual, leaf_has_its_own_kind)
{
    task27::leaf l{};
    EXPECT_EQ(l.kind(), 3);
}

TEST(final_virtual, virtual_dispatch_through_base_pointer)
{
    task27::middle m{};
    task27::base *p = &m;
    /* Dynamic dispatch resolves to middle::rate. */
    EXPECT_EQ(p->rate(), 2);
}

TEST(final_virtual, virtual_dispatch_through_base_pointer_to_leaf)
{
    task27::leaf l{};
    task27::base *p = &l;
    /* leaf inherits middle::rate (which is final). Through a base
     * pointer the dispatch still finds middle::rate. */
    EXPECT_EQ(p->rate(), 2);
}

TEST(final_virtual, virtual_dispatch_through_middle_pointer)
{
    task27::leaf l{};
    task27::middle *p = &l;
    EXPECT_EQ(p->rate(), 2);
}
