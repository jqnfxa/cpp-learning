#include <gtest/gtest.h>

#include <iostream>

#include "solution.hpp"

/*
 * Compile-time block. These fire BEFORE any runtime TEST, so if your
 * functions return the wrong values you'll see the static_assert
 * diagnostic during the build, not the test runner.
 */

/* Standard guarantee: alignof(char) is exactly 1. */
static_assert(task06::alignment_of_char() == 1, "the standard pins alignof(char) at 1");

/* All alignments are at least 1. */
static_assert(task06::alignment_of_int() >= 1, "alignment is at least 1");
static_assert(task06::alignment_of_double() >= 1, "alignment is at least 1");
static_assert(task06::alignment_of_long_double() >= 1, "alignment is at least 1");

/* The student's wrappers must match the `alignof` operator. */
static_assert(task06::alignment_of_char() == alignof(char), "alignment_of_char wraps alignof(char)");
static_assert(task06::alignment_of_int() == alignof(int), "alignment_of_int wraps alignof(int)");
static_assert(task06::alignment_of_double() == alignof(double), "alignment_of_double wraps alignof(double)");
static_assert(task06::alignment_of_long_double() == alignof(long double), "alignment_of_long_double wraps alignof(long double)");
static_assert(task06::alignment_of_mixed() == alignof(task06::mixed), "alignment_of_mixed wraps alignof(mixed)");
static_assert(task06::size_of_mixed() == sizeof(task06::mixed), "size_of_mixed wraps sizeof(mixed)");

/* sizeof is always a multiple of alignof -- a language invariant. */
static_assert(sizeof(char) % alignof(char) == 0, "sizeof(char) % alignof(char) == 0");
static_assert(sizeof(int) % alignof(int) == 0, "sizeof(int) % alignof(int) == 0");
static_assert(sizeof(double) % alignof(double) == 0, "sizeof(double) % alignof(double) == 0");
static_assert(sizeof(long double) % alignof(long double) == 0, "sizeof(long double) % alignof(long double) == 0");
static_assert(task06::size_of_mixed() % task06::alignment_of_mixed() == 0, "sizeof(mixed) is a multiple of alignof(mixed)");

/*
 * The struct's alignment is the max of its members' alignments.
 * mixed has {char, int, char}, so max = alignof(int).
 */
static_assert(task06::alignment_of_mixed() == alignof(int), "alignof(mixed) is the max of member alignments (here: alignof(int))");

/*
 * The classic padding observation: sizeof(mixed) > sum of member sizes.
 * char + int + char = 1 + sizeof(int) + 1 bytes of raw payload, but the
 * compiler inserts padding to put `b` on an int boundary and to round
 * the struct's total size up to a multiple of its alignment.
 */
static_assert(task06::size_of_mixed() > sizeof(char) + sizeof(int) + sizeof(char), "compiler inserts padding inside / at the tail of `mixed`");

TEST(alignof_op, alignof_char_is_one)
{
    EXPECT_EQ(task06::alignment_of_char(), 1u);
}

TEST(alignof_op, alignments_are_at_least_one)
{
    EXPECT_GE(task06::alignment_of_int(), 1u);
    EXPECT_GE(task06::alignment_of_double(), 1u);
    EXPECT_GE(task06::alignment_of_long_double(), 1u);
}

TEST(alignof_op, wrappers_match_alignof_operator)
{
    EXPECT_EQ(task06::alignment_of_int(), alignof(int));
    EXPECT_EQ(task06::alignment_of_double(), alignof(double));
    EXPECT_EQ(task06::alignment_of_long_double(), alignof(long double));
    EXPECT_EQ(task06::alignment_of_mixed(), alignof(task06::mixed));
    EXPECT_EQ(task06::size_of_mixed(), sizeof(task06::mixed));
}

TEST(alignof_op, mixed_alignment_equals_int_alignment)
{
    EXPECT_EQ(task06::alignment_of_mixed(), alignof(int));
}

TEST(alignof_op, mixed_size_includes_padding)
{
    EXPECT_GT(task06::size_of_mixed(), sizeof(char) + sizeof(int) + sizeof(char));
}

TEST(alignof_op, size_is_multiple_of_alignment)
{
    EXPECT_EQ(task06::size_of_mixed() % task06::alignment_of_mixed(), 0u);
}

/*
 * Informational only -- prints the actual values for this platform.
 * Always passes. The plan hint for this step said "print alignment
 * of char, int, double, long double, and a packed-looking struct" --
 * this is that print.
 */
TEST(alignof_op, print_platform_values)
{
    std::cout << "  alignof(char)        = " << task06::alignment_of_char() << "\n";
    std::cout << "  alignof(int)         = " << task06::alignment_of_int() << "\n";
    std::cout << "  alignof(double)      = " << task06::alignment_of_double() << "\n";
    std::cout << "  alignof(long double) = " << task06::alignment_of_long_double() << "\n";
    std::cout << "  alignof(mixed)       = " << task06::alignment_of_mixed() << "\n";
    std::cout << "  sizeof(mixed)        = " << task06::size_of_mixed() << "\n";
    std::cout << "  raw payload of mixed = " << (sizeof(char) + sizeof(int) + sizeof(char)) << " bytes\n";
    std::cout << "  padding inserted     = " << (task06::size_of_mixed() - (sizeof(char) + sizeof(int) + sizeof(char))) << " bytes\n";
}
