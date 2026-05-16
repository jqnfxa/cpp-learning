# 36 — `std::forward` and perfect forwarding

> Spec: <https://en.cppreference.com/cpp/utility/forward>

## What you learn

In step 30 you saw that a named `T &&` variable is itself an lvalue.
That makes forwarding through a template tricky: the function
receives an rvalue, but as soon as it touches the parameter by
name, the rvalue-ness is lost.

```cpp
template <class T>
void wrap(T &&arg)
{
    inner(arg);             /* arg is an lvalue here -- lost the rvalue! */
    inner(std::forward<T>(arg));   /* arg's original category restored */
}
```

`std::forward<T>(arg)` is `arg` cast back to either `T &` or
`T &&` depending on whether `T` was deduced as an lvalue ref or
non-reference. The mechanic relies on **reference collapsing**:

| `T` deduced as... | `T &&` collapses to... | `std::forward<T>(arg)` is... |
|-------------------|------------------------|------------------------------|
| `int` (rvalue arg) | `int &&`               | `int &&` (xvalue)            |
| `int &` (lvalue arg) | `int &`              | `int &` (lvalue)             |

The two combined are called a **forwarding reference** (sometimes
"universal reference") and require:
1. The parameter type to be exactly `T &&` (where `T` is a deduced
   template parameter — `const T &&`, `std::vector<T> &&`, etc.
   *don't* count).
2. `std::forward<T>(...)` to restore the original category at the
   forwarding site.

The two together let you write a function that "forwards" its
arguments perfectly to a constructor / function call: lvalues stay
lvalues, rvalues stay rvalues.

This is the backbone of `std::make_unique`, `emplace_back`, and
many other "construct in place with these args" facilities.

## Task

In `solution.hpp`, implement:

```cpp
template <class T, class... Args>
T factory(Args &&...args);
```

It must construct a `T` from the forwarded arguments. One line:

```cpp
return T(std::forward<Args>(args)...);
```

`tests.cpp` provides a `sink` type whose constructors return
distinct sentinel values depending on whether each parameter
arrived as lvalue or rvalue. Calling `factory<sink>(...)` with
various argument mixes lets the tests verify which sink constructor
was selected — proving lvalue/rvalue category is preserved.

| Call                                                | Picks the sink ctor accepting...     | `.from` |
|-----------------------------------------------------|--------------------------------------|---------|
| `factory<sink>()`                                   | `()`                                 | `0`     |
| `factory<sink>(s)` (where s is a `std::string` lvalue) | `const string &`                  | `1`     |
| `factory<sink>(std::move(s))`                       | `string &&`                          | `2`     |
| `factory<sink>(42)`                                 | `int`                                | `3`     |
| `factory<sink>(42, s)`                              | `int, const string &`                | `4`     |
| `factory<sink>(42, std::move(s))`                   | `int, string &&`                     | `5`     |

## Run

```sh
./verify.sh
```

## Hints

- The implementation is one line:
  ```cpp
  template <class T, class... Args>
  T factory(Args &&...args)
  {
      return T(std::forward<Args>(args)...);
  }
  ```
  Note the **two** parameter-pack expansions: `Args &&...` declares
  the pack, `std::forward<Args>(args)...` forwards it.

- Variadic templates (parameter packs) are step 55 of this plan.
  You haven't *formally* covered them yet, but the pattern here is
  copy-and-adapt. The interesting half of this task is the
  `std::forward` cast — the `...` is plumbing.

- Don't write `std::forward(args)` (no `T`). The function template
  is intentionally hard to call without specifying the type,
  because the type *is* what selects the cast direction.

- Don't use `std::move` instead of `std::forward`. `std::move`
  unconditionally produces an xvalue; for an lvalue argument
  that's *wrong* — it would silently move-construct from something
  the caller still owns.
