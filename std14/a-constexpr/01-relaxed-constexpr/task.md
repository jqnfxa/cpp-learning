# 1 — Relaxed `constexpr` functions

> Spec: <https://en.cppreference.com/cpp/language/constexpr>

## What you learn

C++11 introduced `constexpr` functions, but with a brutal
restriction: the body had to be a **single return
statement**. No local variables. No `if`. No loops. Recursive
expressions only.

```cpp
/* C++11 form */
constexpr long long factorial(long long n)
{
    return n <= 1 ? 1 : n * factorial(n - 1);
}
```

C++14 relaxed this. A `constexpr` function body can now
contain:

- Local variables (`int r = 1;`).
- `if` / `switch` (with normal flow).
- `for`, `while`, `do-while` loops.
- Multiple statements and multiple return paths.
- Mutations on local objects.

So the same factorial becomes the imperative form everyone
already knew how to write:

```cpp
/* C++14 form */
constexpr long long factorial(long long n)
{
    long long r = 1;
    for (long long i = 2; i <= n; ++i)
        r *= i;
    return r;
}
```

Same `constexpr`, same compile-time evaluation, but the body
is normal-looking imperative code.

### Still forbidden in C++14

A few things are still off-limits in a C++14 constexpr body:

- `try` / `catch` (added in C++20).
- `goto`.
- `static` or `thread_local` variables.
- Uninitialized variables.
- Assembly (`asm`).
- Calls to non-constexpr functions when context demands
  constant evaluation.

C++20 and C++23 further relax these, but for C++14 the
restrictions above remain.

### Why this matters

The C++11 single-return rule meant you needed a recursive
expression for any non-trivial logic. Compilers had a
hard time inlining deep recursion at compile time, and you
hit `-fconstexpr-depth` limits quickly. The relaxed form is
both easier to write AND easier for the compiler to
evaluate.

### `constexpr` doesn't mean "evaluated at compile time"

It means "**eligible** for compile-time evaluation if called
with constant arguments." A `constexpr` function called with
a runtime value just runs at runtime. Whether the compiler
picks compile-time evaluation depends on **how the result
is used**:

```cpp
constexpr long long f5 = factorial(5);     /* MUST be at compile time */
long long g = factorial(some_runtime_var); /* runtime */
```

The keyword on a variable (`constexpr long long f5`) is the
strongest force toward compile-time evaluation. `constexpr`
on the function is necessary but not sufficient.

### Recursive vs iterative -- and why iterative wins

The C++11 recursive form needs N levels of recursion to
compute `factorial(N)`. Each level is a separate
constant-evaluation frame, bumping into compilers'
`-fconstexpr-depth` limit (typically 512). For
`factorial(10000)` you'd hit it.

The iterative form has ONE frame, with a loop counter. The
compiler evaluates the loop body N times in the same frame.
No depth limit.

### Mutation of local state IS legal

```cpp
constexpr int sum_to(int n)
{
    int s = 0;
    for (int i = 1; i <= n; ++i) s += i;   /* mutation OK */
    return s;
}
```

The mutation only affects local objects (the local `s`, the
loop counter). The function itself remains pure from the
caller's perspective -- you get the same result for the same
inputs. C++14 distinguished "mutation of local state" from
"observable side effects" -- the former is fine in
constexpr.

### `constexpr` member functions are NOT implicitly `const` (C++14)

That's the next step. C++11 baked `const` into every
`constexpr` member function -- you couldn't write a `constexpr`
setter. C++14 lifted that, which is why we'll see
`constexpr void set_x(int)` in step 2.

## Task

In `solution.hpp`, replace the C++11-style recursive
factorial with the iterative C++14 form. The signature stays
the same:

```cpp
namespace task01
{
    constexpr long long factorial(long long n);
}
```

Use:
- A local `long long r = 1`.
- A `for` loop from 2 to n.
- Multiplication `r *= i` inside the loop.
- Return `r` at the end.

## Run

```sh
./verify.sh
```

## Hints

- The reference body:
  ```cpp
  long long r = 1;
  for (long long i = 2; i <= n; ++i) r *= i;
  return r;
  ```

- `factorial(0)` MUST return 1 (the empty product). The loop
  body doesn't execute when `n < 2`, so `r` stays 1. The stub
  has the wrong base case (returns 0).

- `long long` overflows at `factorial(21)` or so; the tests
  stay safely under `factorial(12)`.

- Keep `constexpr` on the function. Removing it would make
  the `constexpr long long f5 = factorial(5)` declarations in
  the tests fail to compile.

- The same code compiles and runs at runtime too. The whole
  point of `constexpr` is "compile-time IF the args are
  constant, runtime otherwise."

- Tests verify: factorial(0)..(12) values match, plus
  `constexpr long long` variable declarations that force
  compile-time evaluation for n=0, 5, 10.
