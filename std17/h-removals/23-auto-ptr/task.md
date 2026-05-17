# 23 — Removed: `std::auto_ptr`

> Spec: <https://en.cppreference.com/cpp/memory/auto_ptr>

`std::auto_ptr` was REMOVED in C++17. It had broken copy
semantics (copying transferred ownership, leaving the
source null). Use `std::unique_ptr` instead.

```cpp
/* C++03 */
std::auto_ptr<widget> p(new widget(42));
std::auto_ptr<widget> q = p;   /* p is now null -- wrong */

/* C++17 */
std::unique_ptr<widget> p = std::make_unique<widget>(42);
std::unique_ptr<widget> q = std::move(p);   /* explicit move */
```

## Task

Use `std::make_unique<widget>(v)` in the factory; move-
transfer ownership in `move_and_read`.

## Hints

- `std::make_unique` is C++14+ (step 12 of std14).
- `std::move(p)` transfers ownership; `p` is null after.
