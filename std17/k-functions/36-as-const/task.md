# 36 — `std::as_const`

`std::as_const(x)` returns `const T&` view of `x`. Useful
when calling a function that takes `const T&` from a non-
const lvalue, without making a copy.

```cpp
std::vector<int> v = ...;
some_func(std::as_const(v));   /* explicitly passes const ref */
```

## Hints

- Pure type-system tool: produces const-ref without making
  a copy.
- Useful for range-for to iterate a non-const container
  without accidentally modifying.
