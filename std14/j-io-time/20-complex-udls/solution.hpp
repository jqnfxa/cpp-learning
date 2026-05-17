#pragma once

#include <complex>

/*
 * Step 20 -- complex UDLs (C++14). See task.md.
 *
 * C++14 added three UDLs for std::complex:
 *
 *   1i   ->  std::complex<double>(0, 1)
 *   1if  ->  std::complex<float>(0, 1)
 *   1il  ->  std::complex<long double>(0, 1)
 *
 * Combined with real-number arithmetic, they let you write
 * complex numbers naturally:
 *
 *   auto z = 1.0 + 2i;    // 1 + 2i (real=1, imag=2)
 *
 * The scaffold ships with stub values: real numbers without
 * the imaginary part. Tests detect that.
 */

namespace task20
{

using namespace std::complex_literals;

/* TODO: return 1.0 + 2i; */
inline std::complex<double> one_plus_two_i()
{
    return 1.0; /* stub: real only */
}

/* TODO: return 3.0 + 4i;
 * Modulus of 3+4i = sqrt(3^2 + 4^2) = 5.0.
 */
inline std::complex<double> three_plus_four_i()
{
    return 3.0;
}

/* TODO: return std::abs(z) which gives the modulus.
 *
 * For 3+4i, the result is 5.0 (the classic Pythagorean
 * triple). The stub returns just z.real(), which is wrong
 * unless the imaginary part happens to be 0.
 */
inline double modulus(std::complex<double> z)
{
    return z.real();
}

/* TODO: return z.real() * z.real() + z.imag() * z.imag();
 * Same as |z|^2, conjugate*z. Stub returns just real squared.
 */
inline double squared_modulus(std::complex<double> z)
{
    return z.real() * z.real();
}

}
