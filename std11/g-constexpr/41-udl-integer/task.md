# 41 — User-defined literals: integer

> Spec: <https://en.cppreference.com/cpp/language/user_literal>

## What you learn

C++11 lets you define your own literal suffixes. The result is a
syntax that reads like a built-in literal but constructs whatever
type you choose, with whatever conversion you want.

```cpp
constexpr long long operator"" _km(unsigned long long km)
{
    return static_cast<long long>(km * 1000);
}

constexpr long long marathon = 42_km;     /* 42000 meters */
```

The function is called `operator""` followed by the suffix. For
**integer literals** (no decimal point and no exponent), the
*cooked* form takes `unsigned long long`. The cooked form means
"the compiler has already parsed the digits into a number; give
me that number". (There's also a *raw* form that takes the digit
characters as a string — step 44 covers that.)

Three things to know:

1. **The suffix must start with `_`.** All non-underscore suffixes
   are reserved for the standard library (which uses `s` for
   seconds, `ms` for milliseconds, etc., from C++14 onward).

2. **The suffix takes a fixed parameter type.** For integer
   literals it's `unsigned long long` and you can't change it.
   For floats it's `long double`. For strings it's `const char *,
   std::size_t`. (See the next three tasks for each.)

3. **Make it `constexpr`** if you want `42_km` to be usable in
   constant expressions like `static_assert`.

## Task

In `solution.hpp`, implement two integer UDLs:

```cpp
/* 1_km = 1000 (meters). */
constexpr long long operator"" _km(unsigned long long km);

/* 1_h = 3600 (seconds). */
constexpr long long operator"" _h(unsigned long long hours);
```

| Literal     | Returns |
|-------------|---------|
| `1_km`      | `1000`  |
| `0_km`      | `0`     |
| `42_km`     | `42000` |
| `1_h`       | `3600`  |
| `24_h`      | `86400` |

Tests verify the values at both compile time (`static_assert`) and
runtime, including arithmetic combinations like `2_km + 500` and
`1_h * 7`.

## Run

```sh
./verify.sh
```

## Hints

- One-liner each:
  ```cpp
  constexpr long long operator"" _km(unsigned long long km)
  {
      return static_cast<long long>(km * 1000);
  }
  ```
  Cast to `long long` because the multiplication is `unsigned long long`.

- The function signature is *not* template-parameterized in this
  cooked form. The parameter type is fixed by the language to
  `unsigned long long`.

- A UDL with parameter type `unsigned long long` only matches
  *integer* literals — `1.0_km` would not call it (that needs a
  separate floating-point UDL, step 42).

- You can write the suffix with or without a space:
  `operator"" _km` and `operator""_km` are both legal. The
  space is conventional and avoids potential parser ambiguity with
  some future literal suffix.
