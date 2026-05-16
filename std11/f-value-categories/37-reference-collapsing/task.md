# 37 — Reference collapsing

> Spec: <https://en.cppreference.com/cpp/language/reference#Reference_collapsing>

## What you learn

C++ does not allow declaring a reference to a reference directly —
but through templates, `typedef`s, and `decltype`, you *can* end
up with a composite type like `int & &&`. The language defines what
happens then with the **reference collapsing** rules:

| Composite      | Collapses to |
|----------------|--------------|
| `T & &`        | `T &`        |
| `T & &&`       | `T &`        |
| `T && &`       | `T &`        |
| `T && &&`      | `T &&`       |

The rule: **anything with at least one `&` becomes `T &`. Only
`&& &&` survives as `T &&`.**

This is how forwarding references work. A function template:

```cpp
template <class T>
void f(T &&arg);
```

When called with an lvalue of type `int`, type deduction picks
`T = int &`. The parameter type after substitution is
`int & &&` → which collapses to `int &`. So inside `f`, `arg` has
type `int &`.

When called with an rvalue of type `int`, type deduction picks
`T = int`. The parameter type after substitution is `int &&` —
no collapsing needed, still `int &&`.

Two takeaways:

1. **`T &&` is a *forwarding* reference only when `T` is a
   deduced template parameter.** `void f(int && x)` (no template)
   is just an rvalue reference; nothing forwards.
2. **`std::forward<T>` uses these rules to reconstruct the
   original category.** If `T` deduced as `int &`, then `static_cast<T &&>(arg)`
   is `static_cast<int &>(arg)` — an lvalue. If `T` deduced as
   `int`, then `static_cast<T &&>(arg)` is `static_cast<int &&>(arg)`
   — an xvalue. That's the actual machinery you used in step 36.

## Task

In `solution.hpp`, declare two helpers inside `task37`:

```cpp
/* Tag for what T is. Returns one of "lvalue_ref", "rvalue_ref",
 * or "value" based on T (NOT the value category of an expression). */
template <class T>
const char *kind();

/* Forwarding-reference function that returns kind<T>() where T is
 * the *deduced* template parameter. */
template <class T>
const char *deduce(T &&arg);
```

`kind<T>()` reports the *type* `T` (using `is_lvalue_reference` /
`is_rvalue_reference`). `deduce(x)` calls `kind<T>()` where `T` was
deduced from the argument. Together they let `tests.cpp` watch
reference collapsing at work.

Plus 4 `static_assert`s in `tests.cpp` that pin the four collapsing
rules directly via `std::is_same<T &&, ...>::value`.

| Call                                 | Returns       |
|--------------------------------------|---------------|
| `deduce(lvalue_int)`                 | `"lvalue_ref"` |
| `deduce(42)`                         | `"value"`     |
| `deduce(std::move(lvalue_int))`      | `"value"`     |
| `deduce(rvalue_ref_variable)`        | `"lvalue_ref"` (named T&& is lvalue) |

## Run

```sh
./verify.sh
```

## Hints

- `kind<T>()`:
  ```cpp
  return std::is_lvalue_reference<T>::value ? "lvalue_ref"
       : std::is_rvalue_reference<T>::value ? "rvalue_ref"
                                            : "value";
  ```
  The "rvalue_ref" branch is reachable from deliberately-spelled
  call sites (e.g. `kind<int &&>()`), but template deduction of
  `T &&` *never* gives `T = int &&` — only `int &` or `int`.

- `deduce(arg)`: one line.
  ```cpp
  return kind<T>();
  ```

- The static_asserts in `tests.cpp` directly construct `T &&`
  from various spellings of `T` (`int`, `int &`, `int &&`) and
  compare to the expected collapsed type with `std::is_same`.
  Nothing else to do for them.
