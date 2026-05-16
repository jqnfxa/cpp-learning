# 39 — `constexpr` functions (C++11 strict)

> Spec: <https://en.cppreference.com/cpp/language/constexpr>

## What you learn

A **`constexpr` function** is callable both at runtime *and* at
compile time — depending on whether its arguments are themselves
constant expressions. When called with constants, the compiler
evaluates the function and the result is itself a constant
expression (usable as an array bound, in `static_assert`, etc.).

C++11 imposes strict rules on the function body. Inside a
`constexpr` function you may have:

- exactly one `return` statement (plus any number of compile-time-only
  things like `typedef`s and `static_assert`s)
- no local variables
- no `if`/`for`/`while`
- no goto, try, asm

Everything else: forbidden. You compose what you need using the
**ternary operator** and **logical short-circuit operators**, and
you compose iteration via **recursion**.

```cpp
constexpr int factorial(int n)
{
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int six = factorial(3);      /* compile time */
int seven = 7;
int v = factorial(seven);              /* runtime */
```

C++14 will let you write multiple statements, locals, `if`, `for`,
etc., in `constexpr` functions — that's exactly what step 1 of
`std14/plan.md` opens with. For now, *strict* C++11 means one
return statement.

## Task

In `solution.hpp`, implement three `constexpr` functions:

```cpp
constexpr int factorial(int n);    /* n! for n >= 0 */
constexpr int fib(int n);          /* nth Fibonacci, fib(0)=0, fib(1)=1 */
constexpr int gcd(int a, int b);   /* greatest common divisor of |a|, |b| */
```

Each body is a single `return` expression. Use ternary and recursion.

| Call             | Returns | Why                          |
|------------------|---------|------------------------------|
| `factorial(0)`   | `1`     | `0! = 1`                     |
| `factorial(5)`   | `120`   | `5 * 4 * 3 * 2 * 1`          |
| `fib(0)`         | `0`     |                              |
| `fib(1)`         | `1`     |                              |
| `fib(7)`         | `13`    | `0,1,1,2,3,5,8,13`           |
| `gcd(12, 18)`    | `6`     |                              |
| `gcd(7, 0)`      | `7`     | `gcd(n, 0) == n`             |
| `gcd(0, 7)`      | `7`     |                              |
| `gcd(-12, 18)`   | `6`     | sign of result is positive    |

`tests.cpp` verifies each function at compile time with
`static_assert` and at runtime with `EXPECT_EQ`.

## Run

```sh
./verify.sh
```

## Hints

- Factorial: `return n <= 1 ? 1 : n * factorial(n - 1);`
- Fibonacci: `return n < 2 ? n : fib(n - 1) + fib(n - 2);`
- GCD (Euclidean): `return b == 0 ? (a < 0 ? -a : a) : gcd(b, a % b);`
  - When `b == 0`, return `|a|` (handles negative inputs).
  - Otherwise recurse with `gcd(b, a % b)`.
  - `a % b` in C++11 follows the sign of the dividend (truncation),
    which is the same convention modern Euclid expects.

- Do not write `if`, `for`, or local variables. The compiler will
  reject them as not-a-constant-expression in a C++11 `constexpr`
  function.

- All three functions are single-line returns. The `static_assert`s
  in `tests.cpp` fail at the *call site* with "is not a constant
  expression" if your body smuggles in something disallowed.
