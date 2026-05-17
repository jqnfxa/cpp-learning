# 20 — Complex UDLs (`i`, `if`, `il`)

> Spec: <https://en.cppreference.com/cpp/numeric/complex/operator%22%22i>

## What you learn

C++14 added user-defined literals for `std::complex`:

| Suffix | Type                          |
|--------|-------------------------------|
| `1i`   | `std::complex<double>(0, 1)`  |
| `1if`  | `std::complex<float>(0, 1)`   |
| `1il`  | `std::complex<long double>(0, 1)` |

Each makes a pure imaginary number. Real-imaginary
combinations work via `+`:

```cpp
using namespace std::complex_literals;

auto z = 1.0 + 2i;        /* std::complex<double>(1, 2) */
auto f = 1.0f + 2if;      /* std::complex<float>(1, 2)  */
auto l = 1.0L + 2il;      /* std::complex<long double>(1, 2) */
```

Before C++14, you'd write `std::complex<double>(1, 2)` --
correct but verbose, especially in math-heavy code.

### The math intuition

`i` is the imaginary unit: `i * i = -1`. Multiplying by `i`
rotates by 90 degrees in the complex plane.

```cpp
2i * 3i;          /* (2i)(3i) = 6 * i^2 = -6 (real) */
(1+i)^2;          /* (1+i)(1+i) = 1 + 2i + i^2 = 0 + 2i = 2i */
```

The UDLs build naturally on this: `2i` is `0 + 2i`, `3+4i` is
the obvious complex number.

### Operations

`std::complex<T>` supports the standard math:

```cpp
auto z = 3.0 + 4i;
z.real()             /* 3.0 */
z.imag()             /* 4.0 */
std::abs(z)          /* modulus: sqrt(real^2 + imag^2) = 5.0 */
std::norm(z)         /* squared modulus: real^2 + imag^2 = 25.0 */
std::conj(z)         /* conjugate: 3.0 - 4i */
std::arg(z)          /* phase angle in radians */
std::polar(r, theta) /* construct from polar form */
```

Also: `+`, `-`, `*`, `/`, `std::exp`, `std::log`, `std::sin`,
`std::sqrt`, etc. -- all the math functions are overloaded
for complex.

### The classic 3-4-5 triangle

The test uses `3 + 4i` because its modulus is exactly 5
(integer Pythagorean triple). No floating-point fuzziness:

```cpp
std::abs(3.0 + 4i)        /* exactly 5.0 */
std::norm(3.0 + 4i)       /* exactly 25.0 */
```

### Namespace

```cpp
namespace std::complex_literals       /* the UDLs */
namespace std::literals               /* same UDLs, broader */
namespace std::literals::complex_literals
```

`using namespace std::complex_literals;` brings in `i`,
`if`, `il`. The `using namespace std::literals;` brings in
all UDL families (string, chrono, complex).

### Why not just write `std::complex<double>(...)`?

For one or two literals, the explicit form is fine. But in
math-heavy code (DSP, physics simulations, electrical
engineering), you write LOTS of complex numbers:

```cpp
auto h_lowpass = 1.0 / (1.0 + j*omega/cutoff);     /* transfer function */
auto filter_in = signal + 0.01 * sin(omega_n * t);  /* etc. */
```

With UDLs, this stays readable. Without, every literal turns
into a constructor call and the formula becomes opaque.

### `1i` vs the imaginary literal `_Complex_I`

`<complex.h>` (C) and `<ccomplex>` (C++) provide `_Complex_I`
as a macro for the C99 imaginary unit. The C++14 UDL `1i` is
the equivalent for `std::complex<double>`. They're different
types:

- C: `_Complex double` (C99 native type).
- C++: `std::complex<double>` (template, defined in
  `<complex>`).

For C++ code, use the UDL form -- it produces the
template-instantiated type, which is what the rest of
`<complex>` expects.

### Type deduction

```cpp
auto a = 2i;          /* std::complex<double> */
auto b = 2if;         /* std::complex<float> */
auto c = 1.0 + 2i;    /* std::complex<double> (promoted from double + double) */
auto d = 1.0f + 2if;  /* std::complex<float> */
auto e = 1 + 2i;      /* std::complex<double> (int promoted) */
```

The numeric LHS determines the result type. Mixed-precision
expressions follow standard promotion rules.

### Things that look like UDLs but aren't

```cpp
1if              /* complex UDL: 1.0f * i */
1i               /* complex UDL: 1.0 * i */
50ms             /* chrono UDL: 50 milliseconds */
"hello"s         /* string UDL: std::string("hello") */
```

The chrono and string UDLs cover earlier steps (17, 18, 19).
The complex UDL is in this step. All are namespace-scoped to
avoid ambiguity.

## Task

In `solution.hpp`, fill in four functions:

```cpp
inline std::complex<double> one_plus_two_i()
{
    return 1.0 + 2i;
}

inline std::complex<double> three_plus_four_i()
{
    return 3.0 + 4i;
}

inline double modulus(std::complex<double> z)
{
    return std::abs(z);
}

inline double squared_modulus(std::complex<double> z)
{
    return z.real() * z.real() + z.imag() * z.imag();
}
```

`using namespace std::complex_literals;` is in the scaffold.

## Run

```sh
./verify.sh
```

## Hints

- `<complex>` is the header. Already included.

- The UDL `2i` produces `std::complex<double>(0, 2)`. Adding
  a real number gives the standard 1+2i form.

- `std::abs(complex)` is the modulus (sqrt(real^2 + imag^2)).
  Don't confuse with `std::abs(double)` which is the
  absolute value of a real -- they're different overloads
  but the call site looks identical.

- `std::norm(complex)` is the SQUARED modulus (no sqrt).
  Faster than abs when you don't need the sqrt; equivalent
  to `z * std::conj(z)`.

- The 3-4-5 triangle test: `3 + 4i` has modulus exactly 5.
  Exact integer Pythagorean triple, no floating-point noise.

- For `squared_modulus`, the formula `z.real() * z.real() +
  z.imag() * z.imag()` is the same as `std::norm(z)`. You
  could write either; the test uses the explicit form.

- The "multiplication_2i_times_3i_is_minus_6" test pins the
  `i^2 = -1` algebra: 2i * 3i = 6 * i^2 = -6.

- The conjugate test pins `std::conj(1+2i) == 1-2i`.

- For float / long double precision, use `2if` /
  `2il` (suffixes f or l after the i). The tests don't
  exercise these, but the UDLs are there for completeness.

- All these UDLs are constexpr in C++14, but
  `std::complex<T>(...)` is not constexpr for `T == double`
  -- so `constexpr auto z = 1.0 + 2i;` may not compile
  depending on stdlib version. C++20 made complex constexpr;
  earlier versions are runtime-only.
