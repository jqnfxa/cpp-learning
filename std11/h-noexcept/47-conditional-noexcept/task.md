# 47 — Conditional `noexcept`

> Spec: <https://en.cppreference.com/cpp/language/noexcept_spec>

## What you learn

C++11 lets you make a function's `noexcept` *conditional* on a
compile-time boolean expression:

```cpp
void f() noexcept(EXPR);
```

If `EXPR` evaluates to `true`, `f` is noexcept; if `false`, not.
This is how generic code propagates the noexcept-ness of its
template parameters.

The classic example is a generic `swap`:

```cpp
template <class T>
void swap_(T &a, T &b) noexcept(noexcept(T(std::move(a)))
                             && noexcept(a = std::move(b)))
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}
```

Read the outer `noexcept(...)` as the **specifier** (step 45). The
inner `noexcept(...)`s are **operator** invocations (step 46). The
specifier's argument is the compile-time bool the operators
produce. Hence the **`noexcept(noexcept(...))`** idiom you'll see
all over the standard library.

This says: "`swap_` is noexcept iff `T`'s move-construction and
move-assignment are both noexcept." For `int` that's true; for a
class with throwing move ops it's false.

## Why it matters

Library code uses conditional noexcept to inform callers and to
make algorithms scale gracefully:

- `std::vector::push_back` checks the value type's move ops at
  compile time and decides whether to use moves or copies during
  reallocation (step 35).
- Generic facilities like `std::swap`, `std::pair`'s ctors,
  `std::exchange` all propagate noexcept-ness from their members.

Without conditional `noexcept`, you'd have to choose between "lie
that I'm always noexcept (and crash when T throws)" or "always say
I might throw (and lose the performance benefit when T is
noexcept)".

## Task

In `solution.hpp`, implement:

```cpp
template <class T>
void swap_(T &a, T &b) noexcept(/* ... */);
```

The conditional clause must make `swap_` noexcept iff:
- moving from `T` (move ctor) is noexcept, **and**
- assigning a `T &&` to a `T &` (move-assign) is noexcept.

Implementation:

```cpp
template <class T>
void swap_(T &a, T &b) noexcept(noexcept(T(std::move(a)))
                             && noexcept(a = std::move(b)))
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}
```

`tests.cpp` provides three types:

| Type                       | Move ctor      | Move assign    | Expected `noexcept(swap_)` |
|----------------------------|----------------|----------------|-----------------------------|
| `int`                      | trivial-noexcept | trivial-noexcept | `true`                   |
| `task47::nothrow_movable`  | noexcept       | noexcept       | `true`                       |
| `task47::throwing_movable` | not noexcept   | not noexcept   | `false`                      |

For each type, static_asserts verify that `noexcept(swap_<T>(a, b))`
matches the table.

## Run

```sh
./verify.sh
```

## Hints

- The conditional clause:
  ```cpp
  noexcept(noexcept(T(std::move(a)))
        && noexcept(a = std::move(b)))
  ```
  Outer `noexcept(...)` is the specifier (one argument, a bool).
  Two inner `noexcept(...)` are operators (each yields a bool).
  Combined with `&&`.

- `std::move(a)` is in `<utility>`. Include it.

- The body is the same three-line swap from step 31. Just the
  signature is new.

- The test types are defined in `tests.cpp` so `solution.hpp` only
  has the template. tests.cpp pins behavior via static_assert.
