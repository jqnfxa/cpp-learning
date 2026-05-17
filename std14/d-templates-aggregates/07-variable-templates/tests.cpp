#include <gtest/gtest.h>

#include <cmath>
#include <type_traits>

#include "solution.hpp"

namespace t = task07;

TEST(variable_templates, pi_float_close)
{
    EXPECT_NEAR(t::pi<float>, 3.14159265f, 1e-5f);
}

TEST(variable_templates, pi_double_close)
{
    EXPECT_NEAR(t::pi<double>, 3.14159265358979, 1e-12);
}

TEST(variable_templates, pi_long_double_close)
{
    EXPECT_NEAR(static_cast<double>(t::pi<long double>),
                3.14159265358979, 1e-12);
}

TEST(variable_templates, e_float_close)
{
    EXPECT_NEAR(t::e<float>, 2.71828183f, 1e-5f);
}

TEST(variable_templates, e_double_close)
{
    EXPECT_NEAR(t::e<double>, 2.71828182845904, 1e-12);
}

TEST(variable_templates, e_long_double_close)
{
    EXPECT_NEAR(static_cast<double>(t::e<long double>),
                2.71828182845904, 1e-12);
}

TEST(variable_templates, pi_float_type_is_float)
{
    EXPECT_TRUE((std::is_same<decltype(t::pi<float>), const float>::value));
}

TEST(variable_templates, pi_double_type_is_double)
{
    EXPECT_TRUE((std::is_same<decltype(t::pi<double>), const double>::value));
}

TEST(variable_templates, pi_long_double_type_is_long_double)
{
    EXPECT_TRUE((std::is_same<decltype(t::pi<long double>),
                              const long double>::value));
}

TEST(variable_templates, pi_evaluable_at_compile_time)
{
    /* The whole point of constexpr variable templates: they're
     * usable in constant expressions. */
    constexpr double p = t::pi<double>;
    EXPECT_NEAR(p, 3.14159265358979, 1e-12);
}

TEST(variable_templates, e_evaluable_at_compile_time)
{
    constexpr float ef = t::e<float>;
    EXPECT_NEAR(ef, 2.71828183f, 1e-5f);
}

TEST(variable_templates, pi_float_loses_precision_vs_double)
{
    /* Float can't represent pi to double precision. The
     * difference must be > the float epsilon. */
    double diff =
        std::fabs(static_cast<double>(t::pi<float>) - t::pi<double>);
    EXPECT_GT(diff, 1e-9);
    EXPECT_LT(diff, 1e-6);
}

TEST(variable_templates, distinct_instantiations_share_value)
{
    /* Reading pi<double> twice gives the same value -- the
     * variable template instantiates exactly one variable per
     * (template, T) pair, and that variable is constant. */
    EXPECT_EQ(t::pi<double>, t::pi<double>);
}
