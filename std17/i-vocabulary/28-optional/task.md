# 28 — `std::optional<T>`

> Spec: <https://en.cppreference.com/cpp/utility/optional>

C++17's `std::optional<T>` represents "a T or nothing." Use
it for predictable-failure cases instead of:
- Out-parameters + bool return.
- Sentinel values (-1 for "no result").
- Exceptions for normal failure.

```cpp
std::optional<int> parse_int(const std::string& s);
auto v = parse_int("42");
if (v) std::cout << *v << "\n";   /* 42 */
```

## Hints

- `std::nullopt` is the "empty" value.

- `v.has_value()` or `v.operator bool()` to check.

- `*v` or `v.value()` to access (the latter throws if empty).

- `v.value_or(default)` returns the value or a fallback
  (step 29).
