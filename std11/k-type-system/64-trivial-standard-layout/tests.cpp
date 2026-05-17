#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

/* trivial_pod: both. */
static_assert(std::is_trivial<task64::trivial_pod>::value, "trivial_pod must be trivial");
static_assert(std::is_standard_layout<task64::trivial_pod>::value, "trivial_pod must be standard-layout");

/* trivial_not_layout: trivial, not standard-layout. */
static_assert(std::is_trivial<task64::trivial_not_layout>::value, "trivial_not_layout must be trivial");
static_assert(!std::is_standard_layout<task64::trivial_not_layout>::value,
              "trivial_not_layout must NOT be standard-layout");

/* layout_not_trivial: not trivial, standard-layout. */
static_assert(!std::is_trivial<task64::layout_not_trivial>::value, "layout_not_trivial must NOT be trivial");
static_assert(std::is_standard_layout<task64::layout_not_trivial>::value,
              "layout_not_trivial must be standard-layout");

/* neither: not trivial, not standard-layout. */
static_assert(!std::is_trivial<task64::neither>::value, "neither must NOT be trivial");
static_assert(!std::is_standard_layout<task64::neither>::value, "neither must NOT be standard-layout");

TEST(trivial_standard_layout, smoke)
{
    /* The traits are pure metaprogramming; this test exists just so there
     * is a runtime artifact. The static_asserts above do the real work. */
    SUCCEED();
}
