# 21 — `[[maybe_unused]]`

> Spec: <https://en.cppreference.com/cpp/language/attributes/maybe_unused>

## What you learn

`[[maybe_unused]]` silences "unused" warnings on variables,
parameters, functions, types, etc. without changing runtime
behavior:

```cpp
void f([[maybe_unused]] int x) { return; }  /* no unused-param warning */

[[maybe_unused]] int debug_total = a + b;   /* used only in NDEBUG asserts */

[[maybe_unused]] static int helper() { /* ... */ }   /* may go unused */
```

Useful for:
- Debug-only variables (used inside `assert` which is a
  no-op in release builds).
- Stub function parameters that you'll implement later.
- Conditional compilation: a variable used in only one
  `#if` branch.

## Task

Add `[[maybe_unused]]` to the parameter `x` in `do_nothing`
and to a debug variable inside `complex_op`. Runtime
behavior is unchanged; the attribute just silences the
warning.

## Run

```sh
./verify.sh
```

## Hints

- The attribute goes at the declaration:
  `int do_nothing([[maybe_unused]] int x) { ... }`.

- For variables: `[[maybe_unused]] int debug = ...;`.

- The attribute is a compile-time pragma; runtime tests
  pass either way. The pedagogical point is the cleanup of
  warning messages.

- Pre-C++17 idiom: `(void)x;` in the function body to
  silence the warning. The attribute is cleaner.
