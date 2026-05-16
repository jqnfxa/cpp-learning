#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "solution.hpp"

/* Compile-time pins for the five special members. */
static_assert(std::is_default_constructible<task25::unique_resource>::value, "unique_resource must be default-constructible");
static_assert(std::is_move_constructible<task25::unique_resource>::value, "unique_resource must be move-constructible");
static_assert(std::is_move_assignable<task25::unique_resource>::value, "unique_resource must be move-assignable");

static_assert(!std::is_copy_constructible<task25::unique_resource>::value, "unique_resource must NOT be copy-constructible (= delete)");
static_assert(!std::is_copy_assignable<task25::unique_resource>::value, "unique_resource must NOT be copy-assignable (= delete)");

TEST(deleted_functions, default_constructible)
{
    task25::unique_resource r{};
    EXPECT_EQ(r.id(), 7);
}

TEST(deleted_functions, move_constructible)
{
    task25::unique_resource a{};
    task25::unique_resource b{std::move(a)};
    EXPECT_EQ(b.id(), 7);
}

TEST(deleted_functions, move_assignable)
{
    task25::unique_resource a{};
    task25::unique_resource b{};
    b = std::move(a);
    EXPECT_EQ(b.id(), 7);
}
