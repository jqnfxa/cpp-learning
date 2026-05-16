# 19 — Narrowing prevention with `{}`

> Spec: <https://en.cppreference.com/cpp/language/list_initialization>

## What you learn

Recall the three initialization forms:

```cpp
int a = 3.5;     /* copy initialization        */
int b(3.5);      /* direct initialization      */
int c{3.5};      /* list initialization (C++11) */
```

`a` and `b` silently truncate to `3`. `c` **does not compile** in
C++11: list initialization forbids *narrowing conversions* by spec.
The brace form is the form that protects you.

A conversion is narrowing if it loses information at the source type
level. The standard names four cases:

1. floating-point → integer (always narrowing)
2. wider floating-point → narrower (`long double` → `double`,
   `double` → `float`)
3. integer or unscoped enum → floating-point — unless the source is a
   constant expression that's exactly representable
4. wider integer → narrower integer (`long long` → `int`,
   `int` → `short`) — unless the source is a constant expression that
   fits

So:

| Construct                              | C++11 result            |
|----------------------------------------|--------------------------|
| `int x{42};`                           | OK (identity)            |
| `int x{(short)5};`                     | OK (widening)            |
| `long long x{42};`                     | OK (widening)            |
| `int x{3.5};`                          | error (FP → int)         |
| `int x{(long long)5};`                 | error (wider int → narrower int) |
| `short x{(int)5};`                     | error (wider int → narrower int) |
| `int x = 3.5;` *(copy init)*           | OK, silently truncates   |
| `int x(3.5);` *(direct init)*          | OK, silently truncates   |

## Task

Two pieces in `solution.hpp`:

1. **Demonstrate paren-init truncation.** Implement
   `truncate_to_int(double d)` so it uses the C++11 *direct
   initialization* syntax (parens) and returns the integer with the
   fractional part dropped:

   ```cpp
   int truncate_to_int(double d);    /* implement as: return int(d); */
   ```

2. **Pin the brace-init rule with `static_assert`.** A SFINAE trait
   `task19::is_brace_constructible_from<To, From>` is provided in
   `solution.hpp`. `tests.cpp` uses it to assert each row of the table
   above — at compile time. If the trait or the language behavior
   regresses, the build fails.

The implementation of `truncate_to_int` is one line. The
`static_assert`s in `tests.cpp` do the real teaching work.

## Run

```sh
./verify.sh
```

## Hints

- For `truncate_to_int`:
  ```cpp
  return int(d);             /* paren form */
  /* return (int)d;          -- equivalent C cast */
  /* return static_cast<int>(d);  -- more idiomatic for production */
  ```
  All three are equivalent for this case. The point of the task is
  that none of them is rejected — only the brace form, `int{d}`,
  would be.

- Negative tests for narrowing (i.e. "this *shouldn't* compile") live
  inside `tests.cpp` as `static_assert(!is_brace_constructible_from<...>::value, "...")`.
  You don't write any of those; they're provided.

- The trait works by SFINAE on the expression `To{declval<From>()}`.
  Read it as: "is `To{from_value}` a well-formed expression?" If it
  is, the trait inherits from `true_type`. If it isn't (e.g. narrowing
  rejected), the partial specialization is discarded and the trait
  falls back to `false_type`.
