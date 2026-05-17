# 61 — Trailing return type `-> T`

> Spec: <https://en.cppreference.com/cpp/language/function#Function_declaration>

## What you learn

C++11 added a second way to spell a function's return type, after
the parameter list:

```cpp
auto add(int a, int b) -> int { return a + b; }
```

The function is declared `auto`, and the actual return type
follows the parameter list, introduced by `->`. Same semantics as
a normal return type; just spelled in a different place.

The reason to bother: **the parameters are in scope after the
`->`, so you can use `decltype` on them**. With a leading return
type, that's not possible:

```cpp
/* Won't compile: a and b aren't in scope at the leading position. */
template <class A, class B>
decltype(a + b) add(A a, B b);     /* error */

/* Trailing return type: works -- a and b are in scope. */
template <class A, class B>
auto add(A a, B b) -> decltype(a + b);
```

This unlocks **return-type deduction from parameter types** in
generic code. The canonical use is generic arithmetic where you
don't know which operand's type the result will be (or whether
`a + b` even produces something representable by the larger of
the two).

C++14 will introduce **return-type deduction** (just `auto` with
no `-> ...`) so the `decltype` becomes optional. For C++11 the
trailing return is mandatory if you want the deduction.

## Task

In `solution.hpp`, implement three function templates with
trailing return types:

```cpp
template <class A, class B>
auto add(A a, B b) -> decltype(a + b);

template <class A, class B>
auto multiply(A a, B b) -> decltype(a * b);

template <class T>
auto first_element(const std::vector<T> &v) -> decltype(v[0]);
```

| Call                          | Returns | Result type            |
|-------------------------------|---------|------------------------|
| `add(1, 2)`                   | `3`     | `int`                  |
| `add(1, 2.5)`                 | `3.5`   | `double`               |
| `add(1L, 2)`                  | `3L`    | `long`                 |
| `multiply(2, 3)`              | `6`     | `int`                  |
| `multiply(2, 1.5)`            | `3.0`   | `double`               |
| `first_element({10, 20, 30})` | `10`    | reference-to-const `int`|

## Run

```sh
./verify.sh
```

## Hints

- Bodies are one line each:
  ```cpp
  template <class A, class B>
  auto add(A a, B b) -> decltype(a + b)
  {
      return a + b;
  }
  ```

- `first_element` uses `decltype(v[0])`. `vector<T>::operator[]`
  returns a `T &` (or `const T &` if the vector is const). For
  `const std::vector<T> &v`, the result is `const T &`. The
  trailing return type gives you that reference type for free.

- `tests.cpp` pins the deduced return type with
  `std::is_same<decltype(add(1, 2)), int>::value` and friends.

- A common error: writing `auto add(A a, B b) -> decltype(A + B)`.
  That's wrong — `decltype` takes expressions, not types. Use the
  *parameter names* a, b, not the type names A, B.
