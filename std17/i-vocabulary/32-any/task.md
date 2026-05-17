# 32 — `std::any`

> Spec: <https://en.cppreference.com/cpp/utility/any>

A type-erased holder for any copyable value. Unlike variant
(fixed alternative set), any accepts any type at runtime.

```cpp
std::any a = 42;          /* holds int */
a = std::string("hi");    /* now holds string */
int x = std::any_cast<int>(a);   /* throws bad_any_cast if not int */
```

## Hints

- `std::any_cast<T>(any)` throws on type mismatch.
- `std::any_cast<T>(&any)` returns pointer or nullptr (no
  throw).
- Value must be copyable to be stored in any.
