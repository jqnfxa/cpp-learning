#include <gtest/gtest.h>

#include <memory>

#include "solution.hpp"

namespace
{

class shared_ptr_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        task69::release(); /* ensure clean slot between tests. */
    }
};

}

TEST_F(shared_ptr_test, make_value_starts_with_one_owner)
{
    auto p = task69::make_value(42);
    ASSERT_NE(p.get(), nullptr);
    EXPECT_EQ(*p, 42);
    EXPECT_EQ(p.use_count(), 1);
}

TEST_F(shared_ptr_test, stash_increments_use_count)
{
    auto p = task69::make_value(7);
    EXPECT_EQ(p.use_count(), 1);

    long after = task69::stash(p);
    EXPECT_EQ(after, 2);
    EXPECT_EQ(p.use_count(), 2);
}

TEST_F(shared_ptr_test, stashed_returns_same_object)
{
    auto p = task69::make_value(99);
    task69::stash(p);

    {
        auto pulled = task69::stashed();
        ASSERT_NE(pulled.get(), nullptr);
        EXPECT_EQ(pulled.get(), p.get());
        EXPECT_EQ(*pulled, 99);
        /* p, stashed slot, pulled -> three owners. */
        EXPECT_EQ(p.use_count(), 3);
    }
    /* `pulled` is gone; back to two. */
    EXPECT_EQ(p.use_count(), 2);
}

TEST_F(shared_ptr_test, release_drops_back_to_one)
{
    auto p = task69::make_value(1);
    task69::stash(p);
    EXPECT_EQ(p.use_count(), 2);

    bool was_held = task69::release();
    EXPECT_TRUE(was_held);
    EXPECT_EQ(p.use_count(), 1);
}

TEST_F(shared_ptr_test, release_on_empty_returns_false)
{
    EXPECT_FALSE(task69::release());
}

TEST_F(shared_ptr_test, stash_overwrite_drops_previous)
{
    auto a = task69::make_value(10);
    auto b = task69::make_value(20);

    task69::stash(a);
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(b.use_count(), 1);

    long after = task69::stash(b); /* overwrites previous slot */
    EXPECT_EQ(after, 2);
    EXPECT_EQ(a.use_count(), 1); /* slot no longer owns `a`'s object */
    EXPECT_EQ(b.use_count(), 2);
}

TEST_F(shared_ptr_test, multiple_external_owners_count_grows)
{
    auto a = task69::make_value(5);
    auto b = a;
    auto c = a;
    EXPECT_EQ(a.use_count(), 3);

    task69::stash(a);
    EXPECT_EQ(a.use_count(), 4);
}
