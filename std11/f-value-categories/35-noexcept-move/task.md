# 35 — `noexcept` move ops and why containers care

> Spec: <https://en.cppreference.com/cpp/language/noexcept_spec>

## What you learn

When `std::vector<T>` needs more capacity, it allocates a bigger
buffer and transfers the existing elements into it. The transfer
mechanism depends on a property of `T`:

- If `T(T &&)` is **`noexcept`** → vector **moves** the elements.
- If `T(T &&)` is *not* `noexcept` and `T` is copy-constructible
  → vector **copies** the elements (preserving the strong exception
  guarantee on `push_back`: if a move halfway through threw,
  vector would have a partially-moved-from source and no way to
  back out).

In other words: marking your move constructor `noexcept` is *load-
bearing* for performance, not just decoration. A `vector` of types
whose move ctor isn't noexcept will copy on every reallocation —
defeating the entire point of move semantics.

The library function the implementation uses is
`std::move_if_noexcept` (C++11, `<utility>`). It returns an
xvalue if the move ctor is noexcept, otherwise an lvalue — so
overload resolution picks move or copy accordingly.

## The trait that detects this

`std::is_nothrow_move_constructible<T>::value` tells you (and the
compiler, and vector) whether `T(T &&)` is `noexcept`.

| Type `T`'s move ctor declared as... | `is_nothrow_move_constructible<T>::value` |
|--------------------------------------|------------------------------------------|
| `T(T &&) noexcept`                   | `true`                                   |
| `T(T &&)` (no spec)                  | `false`                                  |
| `T(T &&) noexcept(false)`            | `false`                                  |

Defaulted move ctors are `noexcept` *if* every subobject's move
ctor is `noexcept`. For a class with only standard-library members
(`vector`, `string`, `unique_ptr`), `= default` gets you a
`noexcept` move ctor for free.

## Task

`solution.hpp` provides two skeleton classes with explicit copy
constructors (so `std::move_if_noexcept` has a copy fallback to
consider — without one, the trait would unconditionally return
`T &&`). You need to add the move constructor to each, with the
right `noexcept` annotation:

```cpp
class safe_mover
{
public:
    safe_mover() = default;
    safe_mover(const safe_mover &) { }
    safe_mover(safe_mover &&) noexcept;   /* TODO: noexcept */
};

class unsafe_mover
{
public:
    unsafe_mover() = default;
    unsafe_mover(const unsafe_mover &) { }
    unsafe_mover(unsafe_mover &&);        /* TODO: no noexcept */
};
```

The bodies can be empty — the test only inspects traits, not
behavior. The point is the *spec* on the function: `noexcept` vs.
(no spec).

Tests pin:

```cpp
static_assert(std::is_nothrow_move_constructible<safe_mover>::value, "...");
static_assert(!std::is_nothrow_move_constructible<unsafe_mover>::value, "...");
```

Plus a behavioral test that demonstrates how `std::move_if_noexcept`
behaves differently for the two — this is the function vector uses
under the hood.

## Run

```sh
./verify.sh
```

## Hints

- The bodies can be empty:
  ```cpp
  safe_mover(safe_mover &&) noexcept { }
  unsafe_mover(unsafe_mover &&) { }
  ```

- Don't write `noexcept(false)` for `unsafe_mover`. Just leaving
  off the `noexcept` spec is the default, which is *not* noexcept.
  (Destructors are an exception: they default to `noexcept(true)`
  in C++11.)

- `std::move_if_noexcept(x)` returns `T &&` if the move ctor of T
  is noexcept, else `const T &`. You can check at compile time with
  `decltype(std::move_if_noexcept(x))`.
