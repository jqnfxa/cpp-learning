# 107 — duration arithmetic and `std::chrono::duration_cast`

> Spec: <https://en.cppreference.com/cpp/chrono/duration>

## What you learn

Step 106 showed the duration *types*. This step covers the
**conversions and arithmetic** between them.

### Implicit no-loss conversion

When the conversion **cannot lose information** (the target's
tick is a divisor of the source's), it's implicit:

```cpp
std::chrono::milliseconds ms = std::chrono::seconds(2);   /* OK */
/* ms.count() == 2000 */
```

`seconds` ticks at 1, `milliseconds` at 1/1000. 1 / (1/1000) =
1000 is a whole number, so the conversion is lossless --
multiply by 1000.

### Explicit lossy conversion: `std::chrono::duration_cast`

When the conversion **might truncate** (target's tick is
*coarser* than the source's), it's explicit:

```cpp
std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(
    std::chrono::milliseconds(1500));
/* s.count() == 1.  The 500ms remainder is silently dropped. */
```

`duration_cast` performs an integer division. It **always
truncates toward zero**:

| Input          | duration_cast<seconds> |
|----------------|------------------------|
| 1500 ms        | 1 s                    |
| 999 ms         | 0 s                    |
| -1500 ms       | -1 s (rounds toward zero) |
| -999 ms        | 0 s                    |

For different rounding (toward nearest, up, down), C++17 adds
`std::chrono::floor`, `ceil`, `round`. In C++11 you write the
arithmetic by hand if you want something other than truncation.

### Duration division

You can divide:
- A duration by a number (Rep) -> a smaller duration.
- A duration by a duration of the same period -> a Rep
  (dimensionless count).
- Mixed-period durations -> the result is in the *finer*
  period (common-type promotion).

```cpp
auto d = std::chrono::milliseconds(1000) / 4;            /* milliseconds(250) */
auto n = std::chrono::milliseconds(1000) / std::chrono::milliseconds(250);
/* n is long long, equals 4 */
auto m = std::chrono::seconds(1) / std::chrono::milliseconds(250);
/* common-type rule: cast both to ms, divide -> 4 */
```

The duration-by-duration case is what "how many quarter-seconds
in a second?" looks like in code -- it returns the Rep, not a
duration.

### Why `duration_cast<T>(d)` and not `T(d)`?

`T(d)` (i.e. the constructor) **only allows lossless
conversion**. If you write
`std::chrono::seconds(std::chrono::milliseconds(1500))`, the
compiler refuses -- the implicit promotion would lose
information.

`duration_cast<T>(d)` is the explicit "yes I know it might
truncate, do it anyway" call. The asymmetry is a design choice:
*loss happens only when you ask for it*.

### Multiplication and chained units

```cpp
auto sleep_time = std::chrono::milliseconds(100) * 3;   /* 300ms */
auto total = std::chrono::seconds(2) + std::chrono::milliseconds(500);
/* total is milliseconds (the finer unit), equals 2500 */
```

The `+` chooses the common type (the finer one), as in step 106.

## Task

In `solution.hpp`, implement three functions:

```cpp
namespace task107
{
    /* Truncating conversion via std::chrono::duration_cast. */
    std::chrono::seconds to_seconds(std::chrono::milliseconds ms);

    /* No-loss conversion via the implicit (constructor) form. */
    std::chrono::milliseconds to_milliseconds(std::chrono::seconds s);

    /* "How many `chunk` durations fit in `total`?" Returns a Rep
     * (long long), not a duration. */
    long long divide(std::chrono::milliseconds total,
                     std::chrono::milliseconds chunk);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `to_seconds(ms)`:
  ```cpp
  return std::chrono::duration_cast<std::chrono::seconds>(ms);
  ```
  Truncates toward zero.

- `to_milliseconds(s)`:
  ```cpp
  return s;                       /* implicit conversion (constructor) */
  ```
  No `duration_cast` needed -- the conversion is lossless.
  Equivalent to `return std::chrono::milliseconds(s);` if you want
  to be explicit.

- `divide(total, chunk)`:
  ```cpp
  return total / chunk;
  ```
  Same-period duration division returns the Rep.

- The "round-toward-zero" rule matters for negative inputs.
  `duration_cast<seconds>(milliseconds(-1500))` is `seconds(-1)`,
  not `seconds(-2)`. This is C/C++ integer division semantics,
  not "round to floor".

- `duration_cast` is in `<chrono>`. Include it.

- Tests rely on `count()` to extract the integer value after
  the cast for `EXPECT_EQ`. Both sides of the equality use
  duration types so the implicit `==` of `duration` does the
  right thing.

- C++14 added `std::chrono::abs`, `floor`, `ceil`, `round`
  rounding helpers. C++11 only has `duration_cast` (truncating).
  If you need rounding-to-nearest, write
  `duration_cast<seconds>(ms + milliseconds(500))` and live
  with the corner cases.

- `std::ratio_divide<P1, P2>` is the underlying primitive that
  decides if a conversion is lossless. The standard library's
  conversion-eligibility rule is "is the ratio of the two
  periods exactly representable as an integer multiple in the
  target's representation?".
