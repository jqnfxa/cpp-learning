#include <gtest/gtest.h>

#include <cstdint>

#include "solution.hpp"

/* Compile-time guarantees about the type itself. */
static_assert(alignof(task07::cache_line) == 64, "cache_line must be 64-byte aligned (alignas(64))");
static_assert(sizeof(task07::cache_line) == 64, "cache_line must be 64 bytes total");
static_assert(sizeof(task07::cache_line) % alignof(task07::cache_line) == 0, "sizeof is a multiple of alignof (true for every well-formed type)");

TEST(alignas_spec, type_alignment_is_64)
{
    EXPECT_EQ(alignof(task07::cache_line), 64u);
}

TEST(alignas_spec, type_size_is_64)
{
    EXPECT_EQ(sizeof(task07::cache_line), 64u);
}

TEST(alignas_spec, instance_address_is_64_aligned)
{
    auto p = reinterpret_cast<std::uintptr_t>(task07::get_instance());
    EXPECT_EQ(p % 64u, 0u) << "static cache_line should land on a 64-byte boundary";
}

TEST(alignas_spec, array_elements_are_packed_at_64_apart)
{
    /*
     * sizeof(cache_line) == 64 and alignof(cache_line) == 64, so a
     * stack-local array packs adjacent elements with zero padding.
     * &arr[1] - &arr[0] should be exactly 64 bytes.
     */
    task07::cache_line arr[2]{};
    auto p0 = reinterpret_cast<std::uintptr_t>(&arr[0]);
    auto p1 = reinterpret_cast<std::uintptr_t>(&arr[1]);
    EXPECT_EQ(p1 - p0, 64u);
    EXPECT_EQ(p0 % 64u, 0u);
    EXPECT_EQ(p1 % 64u, 0u);
}
