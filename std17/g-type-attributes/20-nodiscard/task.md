# 20 — `[[nodiscard]]`

> Spec: <https://en.cppreference.com/cpp/language/attributes/nodiscard>

## What you learn

`[[nodiscard]]` on a function or type triggers a warning
when the return value is discarded:

```cpp
[[nodiscard]] int allocate();
allocate();         /* WARNING: discarded value */
(void)allocate();   /* explicit cast suppresses */

struct [[nodiscard]] error_code { int v; };
get_status();       /* WARNING: discarded error_code */
```

Useful for:
- Error codes / status returns that should be inspected.
- Allocations (leak if discarded).
- Expensive computations whose only point is the result.

The attribute is observed only at use sites; runtime behavior
is unchanged.

## Task

Mark `must_read` with `[[nodiscard]]` and return `x * 2`.
The `error_code` type already has the type-level attribute;
implement `parse` to return `error_code{x * 3}`.

## Run

```sh
./verify.sh
```

## Hints

- `[[nodiscard]] int func() { ... }` -- function-level.

- `struct [[nodiscard]] T { ... }` -- type-level; ALL
  functions returning T are nodiscard.

- Discarding can be made explicit with `(void)func();` or
  `std::ignore = func();` (C++23 has `[[maybe_unused]]`
  variants).

- The warning is opt-in via -Wunused-result or similar; some
  compilers warn by default at -Wall.
