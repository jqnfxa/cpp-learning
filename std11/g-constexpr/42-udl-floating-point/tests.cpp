#include <gtest/gtest.h>

#include "solution.hpp"

using namespace task42;

/* Compile-time sanity. Exact equality on long double is risky; use
 * a tolerance check via boolean expression. */
static_assert(0.0_deg == 0.0L, "0 degrees = 0 radians");
static_assert(0.0_C == 273.15L, "0 Celsius = 273.15 Kelvin");
static_assert(100.0_C == 373.15L, "100 Celsius = 373.15 Kelvin");

/* pi/2 with tolerance. */
static_assert(90.0_deg > 1.5707963267L && 90.0_deg < 1.5707963272L, "90 degrees ~ pi/2");

TEST(udl_floating, deg_zero_is_zero_radians)
{
    EXPECT_NEAR(static_cast<double>(0.0_deg), 0.0, 1e-12);
}

TEST(udl_floating, deg_ninety_is_half_pi)
{
    EXPECT_NEAR(static_cast<double>(90.0_deg), 1.5707963267948966, 1e-9);
}

TEST(udl_floating, deg_one_eighty_is_pi)
{
    EXPECT_NEAR(static_cast<double>(180.0_deg), 3.14159265358979323846, 1e-9);
}

TEST(udl_floating, deg_three_sixty_is_two_pi)
{
    EXPECT_NEAR(static_cast<double>(360.0_deg), 6.28318530717958647692, 1e-9);
}

TEST(udl_floating, celsius_zero_is_freezing_point)
{
    EXPECT_NEAR(static_cast<double>(0.0_C), 273.15, 1e-12);
}

TEST(udl_floating, celsius_boiling_point)
{
    EXPECT_NEAR(static_cast<double>(100.0_C), 373.15, 1e-12);
}

TEST(udl_floating, celsius_minus_value_via_arithmetic)
{
    /*
     * Note: `-273.15_C` does NOT mean "negative 273.15 Celsius".
     * The unary minus applies *after* the UDL. So -273.15_C is
     * actually `-(273.15_C)` = -(273.15 + 273.15) = -546.30.
     *
     * To express "-273.15 Celsius" you compute via arithmetic on
     * 0.0_C, or you make the value first and then negate the
     * Celsius value before passing through the UDL.
     */
    EXPECT_NEAR(static_cast<double>(0.0_C - 273.15), 0.0, 1e-9);
}

TEST(udl_floating, return_type_is_long_double)
{
    static_assert(std::is_same<decltype(1.0_deg), long double>::value, "1.0_deg has type long double");
    static_assert(std::is_same<decltype(0.0_C), long double>::value, "0.0_C has type long double");
    SUCCEED();
}
