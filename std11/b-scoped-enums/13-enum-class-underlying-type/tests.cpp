#include <gtest/gtest.h>

#include <cstdint>
#include <type_traits>

#include "solution.hpp"

/* sizeof matches the pinned underlying type. */
static_assert(sizeof(task13::status) == 1, "enum class status : std::uint8_t -> 1 byte");
static_assert(sizeof(task13::field) == 2, "enum class field  : std::uint16_t -> 2 bytes");
static_assert(sizeof(task13::id) == 8, "enum class id     : std::uint64_t -> 8 bytes");

/* underlying_type matches what was pinned. */
static_assert(std::is_same<std::underlying_type<task13::status>::type, std::uint8_t>::value, "status's underlying type is std::uint8_t");
static_assert(std::is_same<std::underlying_type<task13::field>::type, std::uint16_t>::value, "field's underlying type is std::uint16_t");
static_assert(std::is_same<std::underlying_type<task13::id>::type, std::uint64_t>::value, "id's underlying type is std::uint64_t");

TEST(enum_underlying_type, status_values)
{
    EXPECT_EQ(static_cast<std::uint8_t>(task13::status::ok), 0);
    EXPECT_EQ(static_cast<std::uint8_t>(task13::status::fail), 1);
}

TEST(enum_underlying_type, field_values)
{
    /* No explicit assignments: defaults are 0, 1, 2 in declaration order. */
    EXPECT_EQ(static_cast<std::uint16_t>(task13::field::one), 0);
    EXPECT_EQ(static_cast<std::uint16_t>(task13::field::two), 1);
    EXPECT_EQ(static_cast<std::uint16_t>(task13::field::three), 2);
}

TEST(enum_underlying_type, id_sentinel_is_all_ones)
{
    EXPECT_EQ(static_cast<std::uint64_t>(task13::id::sentinel), ~0ULL);
    EXPECT_EQ(static_cast<std::uint64_t>(task13::id::sentinel), 0xFFFFFFFFFFFFFFFFULL);
}
