#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

/* int + char -> int (4 > 1). */
static_assert(std::is_same<task104::wider<int, char>::type, int>::value,
              "wider<int, char> must be int");

/* char + long -> long (8 > 1 on most 64-bit platforms; even 4 > 1
 * is enough on LLP64). */
static_assert(std::is_same<task104::wider<char, long>::type, long>::value,
              "wider<char, long> must be long");

/* Tie -> prefer A. */
static_assert(std::is_same<task104::wider<int, int>::type, int>::value,
              "wider<int, int> must be int (tie-breaker -> A)");

/* long long is always at least as wide as int. */
static_assert(std::is_same<task104::wider<long long, int>::type, long long>::value,
              "wider<long long, int> must be long long");

static_assert(std::is_same<task104::wider<int, long long>::type, long long>::value,
              "wider<int, long long> must be long long");

/* short + long -> long. */
static_assert(std::is_same<task104::wider<short, long>::type, long>::value,
              "wider<short, long> must be long");

/* double + char -> double (typically 8 > 1). */
static_assert(std::is_same<task104::wider<double, char>::type, double>::value,
              "wider<double, char> must be double");

/* Same-size on most platforms (e.g. int and unsigned int both 4 bytes):
 * tie -> A. */
static_assert(std::is_same<task104::wider<int, unsigned int>::type, int>::value,
              "wider<int, unsigned int>: same size, tie-breaker -> A");

TEST(conditional, smoke)
{
    /* The static_asserts above do the real work; this exists for
     * ctest to have a runtime artifact. */
    SUCCEED();
}
