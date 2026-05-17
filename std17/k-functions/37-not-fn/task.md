# 37 — `std::not_fn`

> Spec: <https://en.cppreference.com/cpp/utility/functional/not_fn>

Negates a predicate. Replaces `std::not1` / `std::not2`
(deprecated in C++17). Useful with STL algorithms that want
a "false" predicate:

```cpp
auto count = std::count_if(v.begin(), v.end(), std::not_fn(is_positive));
```

## Hints

- Works with any callable.
- `std::not_fn(p)(args...)` is equivalent to `!p(args...)`.
