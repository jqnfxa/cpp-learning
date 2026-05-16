# 44 — User-defined literals: raw form

> Spec: <https://en.cppreference.com/cpp/language/user_literal>

## What you learn

Integer and floating-point UDLs come in two flavors:

- **Cooked form** (steps 41 and 42): the compiler parses the
  literal's digits and gives you a `unsigned long long` /
  `long double`. Easy and the common choice.
- **Raw form** (this step): the compiler gives you the literal's
  *characters* as a null-terminated `const char *`. You do the
  parsing yourself.

When use raw? When the cooked form can't represent what you want
to do. Two common cases:

1. **Custom bases.** The cooked integer form doesn't know about
   binary, ternary, or your-favorite-encoding. Write a raw UDL and
   parse the digit string in whatever base you want:
   ```cpp
   constexpr unsigned long long operator"" _bin(const char *str)
   {
       /* walk str, accumulate binary digits */
   }
   auto mask = 1010'1100_bin;   /* 0xAC */
   ```
2. **Arbitrary-precision integers.** The cooked form maxes out at
   `unsigned long long`. A raw UDL can feed the digit string to a
   bignum constructor without loss.

Two important contract details:

- The function takes **`const char *`** and *no* length parameter.
  The string is null-terminated.
- The compiler decides between cooked and raw based on **whether a
  cooked overload exists**. If only a raw one is defined, the raw
  one is called for *every* integer/float literal with that
  suffix. (Don't define both for the same suffix.)
- The raw form is **only allowed for numeric UDLs** — there is no
  raw form for string literals.

## Task

In `solution.hpp`, implement a raw integer UDL `_bin` that parses
the literal as a base-2 number:

```cpp
constexpr unsigned long long operator"" _bin(const char *str);
```

Rules:
- Each character must be `'0'` or `'1'`. Anything else: undefined
  behavior (tests won't hit those cases).
- Digit separators `'\''` are allowed and ignored (so `1010'1100_bin`
  is valid).
- Return the decoded number as `unsigned long long`.

| Literal              | Returns                  |
|----------------------|--------------------------|
| `0_bin`              | `0`                      |
| `1_bin`              | `1`                      |
| `10_bin`             | `2`                      |
| `1010_bin`           | `10`                     |
| `11111111_bin`       | `255`                    |
| `1010'1100_bin`      | `172` (0xAC, separators ignored) |
| `1000000000_bin`     | `512`                    |

Make it `constexpr` so it works in `static_assert`.

## Run

```sh
./verify.sh
```

## Hints

- The recursive single-return form (C++11 strict `constexpr` from
  step 39) works here:
  ```cpp
  constexpr unsigned long long parse_bin(const char *s, unsigned long long acc)
  {
      return *s == '\0'                                                ? acc
           : *s == '\''                                                ? parse_bin(s + 1, acc)
                                                                       : parse_bin(s + 1, (acc << 1) | (*s - '0'));
  }

  constexpr unsigned long long operator"" _bin(const char *str)
  {
      return parse_bin(str, 0);
  }
  ```
  - End of string: return the accumulator.
  - Apostrophe: skip it.
  - Otherwise: shift left by 1 and OR in the next digit.

- The helper is necessary because C++11 `constexpr` forbids
  locals. The helper carries the accumulator as a parameter,
  which is the recursion trick from step 39.

- The literal is in the source file as digits; the *compiler*
  passes you the textual form. There's no integer-overflow risk in
  the *parsing* itself (you just shift and OR), but
  `1...1` with 64 ones overflows the `unsigned long long` accumulator;
  the tests stay within 31 bits.
