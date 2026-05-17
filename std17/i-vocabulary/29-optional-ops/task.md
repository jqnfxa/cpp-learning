# 29 — Optional ops: `value_or`, `has_value`, `*`, `->`

> Spec: <https://en.cppreference.com/cpp/utility/optional>

The C++17 optional API:

- `v.has_value()` / `bool(v)` — is it set?
- `v.value()` — extract (throws if empty).
- `*v` / `v->member` — extract (UB if empty; no check).
- `v.value_or(default)` — extract or fallback.

```cpp
auto v = parse_int(s);
int x = v.value_or(0);     /* chain with default */
if (v) use(*v);            /* explicit check */
```

## Hints

- `value_or` always returns a value; great for chains.
- `*v` is UB on empty; check first with `if (v)`.
- `value()` throws `std::bad_optional_access` on empty.
