# 01 — `nullptr` and `nullptr_t`

> Spec: <https://en.cppreference.com/cpp/language/nullptr>

## What you learn

Before C++11, `NULL` was a macro expanding to an integer constant (`0`
or `0L`). That made every "null pointer" an `int`/`long` — which lost
overload resolution against integer-parameter overloads.

C++11 introduced **`nullptr`**, a prvalue of type **`std::nullptr_t`**.
It is a pointer-like value that converts to *any* pointer type but
never to `int`. Overload resolution can now tell "I am a null pointer"
apart from "I am the integer zero".

## Task

In `solution.hpp`, implement two overloads of `f` inside namespace
`task01` with these signatures and return values:

| Call         | Picks the overload | Must return |
|--------------|--------------------|-------------|
| `f(0)`       | `f(int)`           | `1`         |
| `f(42)`      | `f(int)`           | `1`         |
| `f(nullptr)` | `f(int *)`         | `2`         |
| `f(&x)`      | `f(int *)`         | `2`         |

If you accidentally write `f(int *p)` returning `1` and `f(int n)`
returning `2`, the tests will fail — they assert the *direction* of
overload resolution.

## Run

```sh
./verify.sh
```

If all four tests pass, the task is done.

## Hints

- The two functions go in `solution.hpp` inside `namespace task01`.
- Use brace-init for the literal return values; either `return 1;` or
  `return {1};` works.
- You don't need to use the parameter inside the function body.
