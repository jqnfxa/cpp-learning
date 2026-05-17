#include <gtest/gtest.h>

#include "solution.hpp"

/* sizeof... -- compile-time. Pin via static_assert. */
static_assert(task55::arity() == 0, "no args -> arity 0");
static_assert(task55::arity(1) == 1, "one arg -> arity 1");
static_assert(task55::arity(1, 2, 3) == 3, "three args -> arity 3");

TEST(parameter_packs, concat_empty)
{
    EXPECT_EQ(task55::concat(), "");
}

TEST(parameter_packs, concat_single_int)
{
    EXPECT_EQ(task55::concat(1), "1");
}

TEST(parameter_packs, concat_three_ints)
{
    EXPECT_EQ(task55::concat(1, 2, 3), "123");
}

TEST(parameter_packs, concat_strings)
{
    EXPECT_EQ(task55::concat("hello ", "world"), "hello world");
}

TEST(parameter_packs, concat_mixed_types)
{
    EXPECT_EQ(task55::concat(1, "+", 2, "=", 3), "1+2=3");
}

TEST(parameter_packs, concat_with_floats)
{
    EXPECT_EQ(task55::concat(3.14), "3.14");
}

TEST(parameter_packs, arity_runtime)
{
    EXPECT_EQ(task55::arity(), 0u);
    EXPECT_EQ(task55::arity(42), 1u);
    EXPECT_EQ(task55::arity("a", "b"), 2u);
    EXPECT_EQ(task55::arity(1, 'c', 3.14, "x"), 4u);
}
