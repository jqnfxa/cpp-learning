# 29 — Value categories: lvalue / rvalue / xvalue / prvalue

> Spec: <https://en.cppreference.com/cpp/language/value_category>

## What you learn

Before C++11 there were two value categories — *lvalue* (anything
with a name or address you could take) and *rvalue* (everything
else). Move semantics needed finer-grained distinctions, so C++11
expanded the model to five categories arranged like this:

```
            expressions
           ┌────────────┐
           │  glvalue   │             rvalue
        ┌──┴──┐      ┌──┴──┐       ┌────┴────┐
       lvalue xvalue xvalue        xvalue    prvalue
```

Three useful categories at the leaves:

| Category   | Examples                                          | "Has a name?" | "Can move from?" |
|------------|---------------------------------------------------|---------------|------------------|
| **lvalue** | `x`, `arr[0]`, `*p`, `f()` returning `T &`, named `T &&` | yes           | no (well, `std::move(...)`) |
| **xvalue** | `std::move(x)`, `T()` cast to `T &&`, `f()` returning `T &&` | no            | yes              |
| **prvalue**| `42`, `T{}`, `f()` returning `T` by value         | no            | yes              |

Some classic gotchas:
- **A named `T &&` variable is itself an lvalue.** `void f(int && r)
  { /* r is lvalue here */ }`. You need `std::move(r)` to turn it
  back into an xvalue when passing it along.
- **`f()` returning `T` is a prvalue.** That's why
  `std::vector<int>{1,2,3}` (a temporary) is movable as
  `std::vector<int> v = std::vector<int>{1, 2, 3};`.

### How to ask the compiler

`decltype((expr))` (with the extra parentheses) encodes the value
category as part of the type:

| Category   | `decltype((expr))` produces |
|------------|-----------------------------|
| lvalue     | `T &`                       |
| xvalue     | `T &&`                      |
| prvalue    | `T` (no reference)          |

Combine with `std::is_lvalue_reference<...>::value` /
`std::is_rvalue_reference<...>::value` and you have a compile-time
oracle.

## Task

In `solution.hpp`, declare inside `task29`:

```cpp
enum class category { lvalue, xvalue, prvalue };

template <class T>
constexpr category classify();
```

`classify<T>()` returns:
- `category::lvalue`  if `T` is an lvalue reference (`U &`),
- `category::xvalue`  if `T` is an rvalue reference (`U &&`),
- `category::prvalue` otherwise (no reference type).

Tests apply it to `decltype((expr))` for a battery of expressions
and assert each one's category at compile time. A successful run
proves you've internalized the table above.

## Run

```sh
./verify.sh
```

## Hints

- C++11's restricted `constexpr` allows one return statement. Use
  the ternary operator to chain the two checks:
  ```cpp
  return std::is_lvalue_reference<T>::value ? category::lvalue
       : std::is_rvalue_reference<T>::value ? category::xvalue
       : category::prvalue;
  ```

- `std::is_lvalue_reference` and `std::is_rvalue_reference` live in
  `<type_traits>`.

- All the *interesting* assertions in `tests.cpp` boil down to
  `decltype((expr))` producing the right reference flavor.
  No runtime measurement is involved -- everything is at compile
  time except for a single sanity test that the enum values are
  distinct.
