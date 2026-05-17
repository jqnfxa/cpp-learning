#include <gtest/gtest.h>

#include <complex>

#include "solution.hpp"

namespace t = task20;
using namespace std::complex_literals;

TEST(complex_udls, one_plus_two_i_real)
{
    auto z = t::one_plus_two_i();
    EXPECT_DOUBLE_EQ(z.real(), 1.0);
}

TEST(complex_udls, one_plus_two_i_imag)
{
    auto z = t::one_plus_two_i();
    EXPECT_DOUBLE_EQ(z.imag(), 2.0);
}

TEST(complex_udls, three_plus_four_i_real)
{
    auto z = t::three_plus_four_i();
    EXPECT_DOUBLE_EQ(z.real(), 3.0);
}

TEST(complex_udls, three_plus_four_i_imag)
{
    auto z = t::three_plus_four_i();
    EXPECT_DOUBLE_EQ(z.imag(), 4.0);
}

TEST(complex_udls, modulus_of_3_plus_4i_is_5)
{
    auto z = std::complex<double>(3.0, 4.0);
    EXPECT_DOUBLE_EQ(t::modulus(z), 5.0);
}

TEST(complex_udls, modulus_of_1_plus_2i)
{
    auto z = std::complex<double>(1.0, 2.0);
    EXPECT_NEAR(t::modulus(z), std::sqrt(5.0), 1e-12);
}

TEST(complex_udls, modulus_of_real)
{
    auto z = std::complex<double>(7.0, 0.0);
    EXPECT_DOUBLE_EQ(t::modulus(z), 7.0);
}

TEST(complex_udls, modulus_of_pure_imag)
{
    auto z = std::complex<double>(0.0, -4.0);
    EXPECT_DOUBLE_EQ(t::modulus(z), 4.0);
}

TEST(complex_udls, squared_modulus_of_3_plus_4i)
{
    auto z = std::complex<double>(3.0, 4.0);
    EXPECT_DOUBLE_EQ(t::squared_modulus(z), 25.0);
}

TEST(complex_udls, plain_i_has_complex_double_type)
{
    auto z = 1.0 + 2i;
    EXPECT_TRUE((std::is_same<decltype(z), std::complex<double>>::value));
}

TEST(complex_udls, multiplication_2i_times_3i_is_minus_6)
{
    /* i^2 = -1, so 2i * 3i = -6. Sanity check on the UDL math. */
    auto z = 2i * 3i;
    EXPECT_DOUBLE_EQ(z.real(), -6.0);
    EXPECT_DOUBLE_EQ(z.imag(), 0.0);
}

TEST(complex_udls, complex_conjugate)
{
    auto z = 1.0 + 2i;
    auto c = std::conj(z);
    EXPECT_DOUBLE_EQ(c.real(), 1.0);
    EXPECT_DOUBLE_EQ(c.imag(), -2.0);
}
