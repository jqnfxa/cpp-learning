# 35 — `std::make_from_tuple`

> Spec: <https://en.cppreference.com/cpp/utility/make_from_tuple>

Construct a T from a tuple of constructor arguments:

```cpp
auto t = std::make_tuple(1, 2);
auto p = std::make_from_tuple<point>(t);   /* point(1, 2) */
```

Like `std::apply` but for type construction instead of
function call.

## Hints

- The type T must have a constructor matching the tuple's
  element types.
- An aggregate's brace-init does NOT count as a "constructor"
  for std::make_from_tuple in C++17 — you'd need a real
  constructor. (C++20 fixed this.)
