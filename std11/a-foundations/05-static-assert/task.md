# 05 — `static_assert`

> Spec: <https://en.cppreference.com/cpp/language/static_assert>

## What you learn

`static_assert` is a compile-time assertion. Its condition must be a
constant expression; if the condition is `false`, the program fails
to compile and the message (a string literal) is the diagnostic.

```cpp
static_assert(sizeof(int) >= 4, "this code assumes 32-bit int");
static_assert(sizeof(void *) == sizeof(std::size_t), "tagged-ptr trick won't work here");
```

C++11 *requires* the message; C++17 makes it optional.

The interesting half of this task is the *other* C++11 feature it
needs: **`constexpr`**. A function you want to call inside
`static_assert(...)` must itself be `constexpr` — and in C++11 a
`constexpr` function body is restricted to a single `return`
statement. No locals, no `if`, no `for`. You compose with the
ternary operator and short-circuiting logical operators.

(C++14 relaxes this; we'll exercise the relaxed form in `std14/`.)

## Task

In `solution.hpp`, implement:

```cpp
namespace task05 {
    constexpr bool is_power_of_two(unsigned long long n);
}
```

Returns `true` iff `n` is a power of two: `1, 2, 4, 8, 16, ...`.
`0` is **not** a power of two.

The body must be a single return statement so the function works as
a `constexpr` under the C++11 rules.

| Input          | Result  | Why                                |
|----------------|---------|------------------------------------|
| `0`            | `false` | special case                       |
| `1`            | `true`  | `2^0`                              |
| `2`            | `true`  | `2^1`                              |
| `3`            | `false` | `0b11`                             |
| `4`            | `true`  | `2^2`                              |
| `6`            | `false` | `0b110`                            |
| `1024`         | `true`  | `2^10`                             |
| `1023`, `1025` | `false` | one shy and one over `2^10`        |
| `1ULL << 63`   | `true`  | highest bit set, still power of 2  |
| `ULLONG_MAX`   | `false` | all bits set                       |

`tests.cpp` checks all of these twice — once at compile time with
`static_assert`, once at runtime with `EXPECT_*`. If your function
isn't really `constexpr` (or returns a non-constant expression), the
compile-time block fails first and you'll see the error before any
runtime test runs.

## Run

```sh
./verify.sh
```

## Hints

- The bit trick: a *positive* integer is a power of two iff
  `n & (n - 1)` is `0`. Handle `n == 0` separately because it would
  otherwise pass that test.
- Express it as one return statement using `&&`. Short-circuit
  evaluation lets you write `n != 0 && (n & (n - 1)) == 0`.
- `unsigned long long` (from step 02) is the right type. On
  `unsigned`, `n - 1` is defined for any `n` (it wraps cleanly to
  `ULLONG_MAX` when `n == 0`), so the bit trick is well-defined even
  in the zero case — the short-circuit `n != 0` just guards correctness.
- C++11's `constexpr` forbids `if`, local variables, and loops in
  function bodies. One return statement, composed.
