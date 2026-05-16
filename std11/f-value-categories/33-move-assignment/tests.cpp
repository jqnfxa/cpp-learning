#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "solution.hpp"

/* Compile-time pins. */
static_assert(std::is_move_assignable<task33::buffer>::value, "buffer must be move-assignable");
static_assert(std::is_nothrow_move_assignable<task33::buffer>::value, "move-assign must be noexcept");
static_assert(!std::is_copy_assignable<task33::buffer>::value, "buffer must not be copy-assignable");

TEST(move_assign, transfers_ownership)
{
    task33::buffer a{5};
    a.data()[0] = 100;
    a.data()[4] = 999;
    int *original_ptr = a.data();

    task33::buffer b;
    b = std::move(a);

    ASSERT_EQ(b.size(), 5u);
    ASSERT_NE(b.data(), nullptr);
    EXPECT_EQ(b.data(), original_ptr);
    EXPECT_EQ(b.data()[0], 100);
    EXPECT_EQ(b.data()[4], 999);

    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.data(), nullptr);
}

TEST(move_assign, releases_target_old_data)
{
    /*
     * If move-assign were just memcpy, the old data_ in `target`
     * would leak. unique_ptr::operator=(&&) releases first, then
     * steals. Test: assigning a smaller buffer into a previously
     * larger buffer leaves us with the smaller size.
     */
    task33::buffer target{10};
    task33::buffer source{3};
    source.data()[0] = 42;

    target = std::move(source);

    ASSERT_EQ(target.size(), 3u);
    ASSERT_NE(target.data(), nullptr);
    EXPECT_EQ(target.data()[0], 42);
}

TEST(move_assign, self_move_is_safe)
{
    /*
     * a = std::move(a) must leave a valid. The defensive form keeps
     * a unchanged.
     */
    task33::buffer a{4};
    a.data()[0] = 7;
    a.data()[3] = 21;

    a = std::move(a);

    EXPECT_EQ(a.size(), 4u);
    ASSERT_NE(a.data(), nullptr);
    EXPECT_EQ(a.data()[0], 7);
    EXPECT_EQ(a.data()[3], 21);
}

TEST(move_assign, returns_reference_to_self)
{
    task33::buffer a;
    task33::buffer b{3};
    task33::buffer &ref = (a = std::move(b));
    EXPECT_EQ(&ref, &a);
}
