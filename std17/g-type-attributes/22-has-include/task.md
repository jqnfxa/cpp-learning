# 22 — `__has_include`

> Spec: <https://en.cppreference.com/cpp/preprocessor/include>

## What you learn

`__has_include(<header>)` is a preprocessor expression that
evaluates to 1 if `<header>` is available, 0 otherwise.
Useful for headers that adapt to the available standard
library:

```cpp
#if __has_include(<optional>)
#  include <optional>
#  define HAS_OPTIONAL 1
#else
#  include "my_optional.hpp"
#  define HAS_OPTIONAL 0
#endif
```

This lets a header library support pre-C++17 compilers
without compile errors.

## Task

Implement `make_optional_int(value)` returning
`std::optional<int>{value}`. The `has_optional()` and
`has_nonexistent_header()` helpers exercise `__has_include`.

## Run

```sh
./verify.sh
```

## Hints

- `__has_include(<header_name>)` works only in preprocessor
  expressions (`#if ...`).

- For absent headers, the expression is 0 (false), so the
  `#else` branch compiles.

- Combined with feature-test macros (`__cpp_lib_optional`,
  `__cpp_concepts`), you can detect both header AND feature
  availability.

- On modern compilers, `<optional>` is always available in
  C++17 mode; the test's `has_optional` returns true.
