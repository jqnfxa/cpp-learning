# 48 — Lambdas: basic syntax and call

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

A **lambda expression** is C++11's syntactic way to create an
unnamed function-like object at the point you need it. The basic
shape:

```cpp
[captures](parameters) -> return_type { body }
```

- `[]` — *capture list* (empty for this step; step 49 introduces
  captures).
- `(parameters)` — parameter list. Can be empty: `[](){...}`.
- `-> return_type` — optional trailing return type. Usually
  omitted; the compiler infers it from the `return` statements.
- `{ body }` — function body. May `return` like any function.

The whole expression has an *anonymous type* unique to that
lambda. You typically store it in `auto`:

```cpp
auto inc = [](int x) { return x + 1; };
int y = inc(5);     /* 6 */
```

A captureless lambda is *implicitly convertible* to a function
pointer with the matching signature, which makes it usable
wherever the C API expects a callback.

## The IIFE pattern

You can call a lambda right where you define it. The "immediately
invoked function expression":

```cpp
const int x = [](){ return 42; }();
/*           ^^^^^^^^^^^^^^^^^^ defines lambda
                                ^^ calls it */
```

Useful for computing a value with some local state, in a single
expression. C++ lacks a "do block" otherwise.

## Task

In `solution.hpp`, implement three functions, each using a lambda
expression inside its body:

```cpp
int inc(int x);            /* uses [](int) {...} */
int sum(int a, int b);     /* uses [](int, int) {...} */
int compute_constant();    /* IIFE returning 42 */
```

Implementations:

```cpp
inline int inc(int x)
{
    auto add_one = [](int y) { return y + 1; };
    return add_one(x);
}

inline int sum(int a, int b)
{
    auto add = [](int p, int q) { return p + q; };
    return add(a, b);
}

inline int compute_constant()
{
    return [](){ return 42; }();
}
```

| Call                      | Returns |
|---------------------------|---------|
| `inc(5)`                  | `6`     |
| `inc(0)`                  | `1`     |
| `inc(-1)`                 | `0`     |
| `sum(3, 4)`               | `7`     |
| `sum(-1, 1)`              | `0`     |
| `compute_constant()`      | `42`    |

## Run

```sh
./verify.sh
```

## Hints

- Lambdas don't need a name unless you want to call them more than
  once. For one-off use, the IIFE form is concise:
  ```cpp
  int x = [](int n) { return n * 2; }(10);  /* 20 */
  ```

- The return type is inferred from the body. For a single-return
  body like ours, the deduced type is `decltype(body_expression)`.
  In `inc`, the lambda returns `int` (from `y + 1`).

- The compiler is free to inline the lambda's body in many
  cases — `inc(5)` may compile down to `return 5 + 1;` with no
  call at all. The lambda is a *language* construct, not
  necessarily a runtime function call.

- Step 49 covers captures (the `[]` part). Step 52 covers
  `mutable` lambdas. For now, `[]` (empty) is all you need.
