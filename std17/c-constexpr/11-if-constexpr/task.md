# 11 — `if constexpr`

> Spec: <https://en.cppreference.com/cpp/language/if>

## What you learn

`if constexpr (condition)` selects ONE branch at compile
time based on a `constexpr` condition. The unselected branch
is **discarded** -- it's not compiled for that
instantiation. This replaces SFINAE + tag-dispatch for most
compile-time branching:

```cpp
template <class T>
std::string describe(T const &v) {
    if constexpr (std::is_integral_v<T>)
        return "int:" + std::to_string(v);
    else if constexpr (std::is_floating_point_v<T>)
        return "float:" + std::to_string(v);
    else
        return "other";
}
```

For each `T`, exactly one branch is compiled. The
`else if constexpr` chain can be arbitrarily long.

### Why "discarded" matters

In a normal `if`, both branches must compile -- so
`if (is_integral<T>::value) to_string(v); else ...` would
fail when `T` is a non-arithmetic type, because
`std::to_string(some_class)` doesn't compile.

With `if constexpr`, the `else` branch is NEVER COMPILED for
integral types. Calls in the discarded branch don't need
to be valid for the current `T`. This is the SFINAE
replacement.

### When NOT to use it

- For runtime conditions: use plain `if`. `if constexpr`'s
  condition MUST be a compile-time constant.
- For exhaustive dispatch on enum/integer values: `switch`
  is clearer when you have many cases.
- For "always inline this branch": the optimizer handles
  that; `if constexpr` is about correctness, not speed.

## Task

In `solution.hpp`, implement `describe` and `size_of` using
`if constexpr` to dispatch on the type traits.

## Run

```sh
./verify.sh
```

## Hints

- `std::is_integral_v<T>` for int types (bool, char, int,
  long, ...).

- `std::is_floating_point_v<T>` for float, double, long
  double.

- `std::is_same_v<T, std::string>` to detect a specific
  type.

- `std::is_arithmetic_v<T>` is `is_integral || is_floating_point`.

- `std::to_string` works for integers and floats. For
  integers it's exact; for doubles it uses sprintf("%f")
  which gives 6 decimal places.

- The "describe_double_starts_with_float" test only checks
  the prefix to avoid floating-point formatting issues.

- `(void)v;` silences unused-parameter warnings in the
  `else` branch (where v isn't used).

- The unselected branch isn't compiled -- so the
  `to_string(int*)` that would be ill-formed in the
  pointer case doesn't matter; the compiler picks the
  "other" branch.

- Combine with C++17 trait `_v` aliases (`is_integral_v`)
  for concise expressions. The `_v` form is the
  variable-template version of `::value`.

- This is the single most readable C++17 feature for
  compile-time branching -- write it instead of SFINAE
  whenever possible.
