# 25 — Removed: dynamic exception specifications

C++17 removed `throw(X, Y)` exception specifications and
`std::unexpected`. Use:

- `noexcept` — function guarantees no exceptions.
- `noexcept(false)` — function may throw (the default; no
  spec needed).

`throw()` is deprecated since C++11, removed in C++17. Use
`noexcept` instead.

## Hints

- `noexcept(false)` is implicit if you don't write anything.
- `noexcept` on a function makes terminate() run if an
  exception tries to escape.
