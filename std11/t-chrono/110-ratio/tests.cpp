#include <gtest/gtest.h>

#include <ratio>

#include "solution.hpp"

/* The headline: 1/3 + 1/6 == 1/2. */
static_assert(std::ratio_equal<task110::one_half, std::ratio<1, 2>>::value,
              "one_half must equal 1/2");

/* Numerator and denominator are accessible directly. */
static_assert(task110::one_half::num == 1, "one_half::num must be 1");
static_assert(task110::one_half::den == 2, "one_half::den must be 2");

/* Ratio is in lowest terms -- the unreduced 3/6 form would have ::num
 * == 3, ::den == 6. The std::ratio template auto-reduces, so the
 * student's solution should naturally land at 1/2. */
static_assert(task110::one_half::num != 3, "must be reduced, not 3/6");
static_assert(task110::one_half::den != 6, "must be reduced, not 3/6");

/* The reverse: 1/2 - 1/6 == 1/3, just to exercise ratio_subtract. */
static_assert(
    std::ratio_equal<std::ratio_subtract<std::ratio<1, 2>, std::ratio<1, 6>>,
                     std::ratio<1, 3>>::value,
    "1/2 - 1/6 must equal 1/3");

/* ratio_multiply: 2/3 * 3/4 == 1/2 == one_half. */
static_assert(
    std::ratio_equal<std::ratio_multiply<std::ratio<2, 3>, std::ratio<3, 4>>,
                     task110::one_half>::value,
    "2/3 * 3/4 must equal one_half (== 1/2)");

TEST(ratio, smoke)
{
    /* All validation is via the static_asserts above; this test
     * exists only so ctest has a runtime artifact to run. */
    SUCCEED();
}
