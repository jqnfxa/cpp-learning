# 42 — User-defined literals: floating-point

> Spec: <https://en.cppreference.com/cpp/language/user_literal>

## What you learn

Floating-point UDLs follow the same pattern as integer UDLs
(step 41) but with a different cooked parameter type: **`long
double`**. Any literal with a decimal point or an exponent goes
through the floating overload.

```cpp
constexpr long double operator"" _deg(long double deg)
{
    return deg * 3.14159265358979323846L / 180.0L;
}

constexpr long double right_angle = 90.0_deg;   /* ~1.5708 (pi/2) */
```

Two things to remember:

1. **The parameter is always `long double`.** You can't pick
   `float` or `double` for the cooked form. (Use a separate
   integer UDL for whole-number convenience; or — what people
   usually do — write `45.0_deg` instead of `45_deg`.)

2. **`long double` is at least as wide as `double` and typically
   provides extra precision** (on x86_64 Linux, 80-bit extended
   precision padded to 16 bytes). Casting down to `double` loses
   that precision, which matters for trig and similar
   computations. Keep things as `long double` until you have to
   narrow.

## Task

In `solution.hpp`, implement two floating-point UDLs:

```cpp
/* Convert degrees to radians. */
constexpr long double operator"" _deg(long double deg);

/* Convert a Celsius temperature to Kelvin. */
constexpr long double operator"" _C(long double celsius);
```

| Literal       | Returns                          |
|---------------|----------------------------------|
| `0.0_deg`     | `0.0L`                            |
| `90.0_deg`    | π/2 ≈ `1.5707963...`              |
| `180.0_deg`   | π ≈ `3.1415926...`                |
| `360.0_deg`   | 2π ≈ `6.2831853...`               |
| `0.0_C`       | `273.15L` (Kelvin)                |
| `100.0_C`     | `373.15L`                         |
| `0.0_C - 273.15` | `0.0L` (absolute zero, computed via subtraction — see hints) |

Use `3.14159265358979323846L` as π. Tests compare with a tolerance
of `1e-9` so you don't have to match a specific representation.

## Run

```sh
./verify.sh
```

## Hints

- Implementations:
  ```cpp
  constexpr long double operator"" _deg(long double deg) {
      return deg * 3.14159265358979323846L / 180.0L;
  }
  constexpr long double operator"" _C(long double celsius) {
      return celsius + 273.15L;
  }
  ```

- Floating-point literals are matched by your floating UDL.
  Integer literals are matched by an integer UDL (the previous
  step). If you write `90_deg` (no decimal point), the compiler
  will look for an *integer* `_deg` UDL — and complain that there
  isn't one.

- Tests use `EXPECT_NEAR(...)` and `static_assert(... > x && ... < y)`
  pairs for floating comparisons. Don't try to make `90.0_deg`
  exactly `1.57079632679...` — floating-point arithmetic isn't
  exact; you just need to be inside the tolerance window.

- Trailing `L` on numeric literals marks them as `long double`
  (`3.14L`) rather than `double`. Without it, the constant gets
  cast to `long double` during the expression, which is fine, but
  the `L` makes the intent explicit.

- **UDL gotcha: signed literals.** `-273.15_C` doesn't pass
  `-273.15` to the UDL. The unary minus applies *after* the UDL is
  done, so `-273.15_C` is `-(273.15 + 273.15) = -546.30`. To
  express "negative Celsius" do arithmetic: `0.0_C - 273.15` for
  absolute zero. Same trap applies to integer UDLs.
