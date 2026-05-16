#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "solution.hpp"

/* Compile-time pins. */
static_assert(!std::is_copy_constructible<task32::buffer>::value, "buffer must not be copy-constructible");
static_assert(std::is_move_constructible<task32::buffer>::value, "buffer must be move-constructible");
static_assert(std::is_nothrow_move_constructible<task32::buffer>::value, "move ctor must be noexcept");

TEST(move_ctor, default_buffer_is_empty)
{
    task32::buffer b;
    EXPECT_EQ(b.size(), 0u);
    EXPECT_EQ(b.data(), nullptr);
}

TEST(move_ctor, sized_buffer_allocates_zero_initialized_storage)
{
    task32::buffer b{4};
    ASSERT_EQ(b.size(), 4u);
    ASSERT_NE(b.data(), nullptr);
    for (std::size_t i = 0; i < b.size(); ++i)
    {
        EXPECT_EQ(b.data()[i], 0);
    }
}

TEST(move_ctor, writes_through_data_persist)
{
    task32::buffer b{3};
    b.data()[0] = 10;
    b.data()[1] = 20;
    b.data()[2] = 30;
    EXPECT_EQ(b.data()[0], 10);
    EXPECT_EQ(b.data()[1], 20);
    EXPECT_EQ(b.data()[2], 30);
}

TEST(move_ctor, move_transfers_ownership)
{
    task32::buffer a{5};
    a.data()[0] = 100;
    a.data()[4] = 999;

    int *original_ptr = a.data();

    task32::buffer b{std::move(a)};

    /* Destination has the data. ASSERT first so we don't deref
     * a null pointer if the move ctor is broken. */
    ASSERT_EQ(b.size(), 5u);
    ASSERT_NE(b.data(), nullptr);
    EXPECT_EQ(b.data(), original_ptr);
    EXPECT_EQ(b.data()[0], 100);
    EXPECT_EQ(b.data()[4], 999);

    /* Source is empty and safe to inspect. */
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.data(), nullptr);
}

TEST(move_ctor, moved_from_buffer_can_be_destroyed)
{
    task32::buffer a{8};
    task32::buffer b{std::move(a)};
    /* When this test exits, both buffers' destructors run on
     * potentially moved-from a (unique_ptr is null) and on b
     * (unique_ptr holds the storage). Neither should crash. */
    SUCCEED();
}
