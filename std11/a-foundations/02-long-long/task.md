# 02 — `long long`

> Spec: <https://en.cppreference.com/cpp/language/types>

## What you learn

`int` is allowed to be as narrow as 16 bits and in practice is 32 bits
on every mainstream platform. That's ±2.1 billion at most. Before C++11
there was no standard built-in *name* for "signed integer at least 64
bits wide" — you had to reach for `int64_t` (which is from C99,
optional in C++03) or platform-specific extensions.

C++11 added the standard built-in types **`long long`** (signed) and
**`unsigned long long`**, both **guaranteed at least 64 bits**, with
the literal suffixes **`LL`** and **`ULL`**.

The trap this task is built around: in an expression like `a * b`,
where `a` and `b` are both `int`, the multiplication happens in `int`.
If the mathematical product exceeds `INT_MAX`, the result has already
overflowed by the time you cast it to `long long`. You have to promote
one operand *before* the multiplication.

```cpp
int a = 100'000, b = 100'000;
long long bad  = a * b;                              // int*int overflows, then converts
long long good = static_cast<long long>(a) * b;      // long long * int, no overflow
```

(Note: signed integer overflow in C++ is undefined behavior, so "bad"
may give any result. On most platforms with wrap-around you'd see
`1'410'065'408`.)

## Task

In `solution.hpp`, implement:

```cpp
namespace task02 {
    long long mul64(int a, int b);
}
```

It must return the mathematical product of `a` and `b` as a 64-bit
integer, correctly for the full input range of `int` × `int`.

| Call                    | Must return       |
|-------------------------|-------------------|
| `mul64(2, 3)`           | `6`               |
| `mul64(0, 100)`         | `0`               |
| `mul64(-3, 7)`          | `-21`             |
| `mul64(-4, -5)`         | `20`              |
| `mul64(100000, 100000)` | `10000000000LL`   |
| `mul64(INT_MAX, 2)`     | `4294967294LL`    |
| `mul64(INT_MIN, -1)`    | `2147483648LL`    |

`tests.cpp` also pins the language guarantees about `long long` with
`static_assert` so you can see how the standard formalizes the type's
size.

## Run

```sh
./verify.sh
```

## Hints

- The correct body is one expression with one cast.
- `return (long long)(a * b);` is **wrong**: the multiplication has
  already overflowed `int` by the time the cast runs.
- The literal `10000000000` (without `LL`) wouldn't even compile —
  it's too big for `int`. That's why `LL` exists.
