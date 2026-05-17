# 30 — `std::variant<Ts...>`

> Spec: <https://en.cppreference.com/cpp/utility/variant>

A type-safe tagged union. The variant holds exactly one of
its alternatives at a time; `.index()` tells you which.

```cpp
using json_value = std::variant<std::nullptr_t, bool, double, std::string>;

json_value v = 3.14;       /* holds double */
v = "hello";               /* now holds string */

if (std::holds_alternative<bool>(v)) { ... }
double d = std::get<double>(v);   /* throws if not double */
```

## Hints

- Each alternative has a fixed index (0-based) by
  declaration order.
- `std::get<T>(v)` extracts by type; `std::get<I>(v)` by
  index.
- `std::get_if<T>(&v)` returns pointer or nullptr (no
  throw).
