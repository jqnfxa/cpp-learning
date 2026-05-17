# 12 — `constexpr` lambdas

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

C++17 made lambda's `operator()` **implicitly `constexpr`**
when its body is a constant expression. Combined with `constexpr` on the lambda variable, you can use lambdas in
constant-expression contexts:

```cpp
constexpr auto square = [](int x) { return x * x; };
static_assert(square(5) == 25);

constexpr int answer = square(7);   /* 49 -- compile-time */
```

Before C++17, lambdas were never constexpr (the call
operator wasn't). You'd write a function template instead.

### Explicit `constexpr` lambda

C++17 lets you write `constexpr` ON the lambda explicitly:

```cpp
auto sq = [](int x) constexpr { return x * x; };
```

The `constexpr` after the parameter list forces the call
operator to be constexpr. If the body isn't constant-
expression-compatible, you get a compile error.

In most cases the implicit form (deduced) is sufficient.

## Task

Implement `sum_via_lambda` and `factorial_via_lambda` -- both
constexpr functions that use lambdas internally and produce
values usable at compile time.

```cpp
inline constexpr int sum_via_lambda(int a, int b) {
    constexpr auto add = [](int x, int y) { return x + y; };
    return add(a, b);
}

inline constexpr long long factorial_via_lambda(int n) {
    constexpr auto step = [](long long acc, int i) { return acc * i; };
    long long acc = 1;
    for (int i = 2; i <= n; ++i) acc = step(acc, i);
    return acc;
}
```

## Run

```sh
./verify.sh
```

## Hints

- The lambda is declared with `constexpr auto add = ...`,
  making it usable in constant expressions inside the
  function.

- The outer function is also `constexpr`, so the WHOLE thing
  is usable at compile time.

- Tests verify both runtime values and `constexpr int s =
  sum_via_lambda(10, 20)` to force compile-time evaluation.

- For C++14 you'd write a function template; for C++17 the
  lambda form is more compact and works in-place.
