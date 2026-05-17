# 31 — `std::visit`

> Spec: <https://en.cppreference.com/cpp/utility/variant/visit>

Dispatch on a variant's current alternative. The callable
must accept ALL alternatives (typically a generic lambda
with `if constexpr` branches, or an overload-set helper).

```cpp
std::visit([](auto const& x) {
    if constexpr (std::is_same_v<std::decay_t<decltype(x)>, int>)
        return "int:" + std::to_string(x);
    else
        return "str:" + x;
}, v);
```

## Hints

- `std::visit(visitor, variant)` — single-variant form.
- The visitor's return type must be the same for all
  alternatives (or unique via auto-return-deduction).
- `if constexpr` lets you give different behavior per
  alternative.
