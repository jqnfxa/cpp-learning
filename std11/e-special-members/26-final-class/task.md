# 26 — `final` on classes

> Spec: <https://en.cppreference.com/cpp/language/final>

## What you learn

C++11 introduced two identifier-like contextual keywords:
**`final`** and **`override`** (next step). Both are designed to
catch class-hierarchy mistakes at compile time. `final` has two
placements:

1. **On a class** — forbids further derivation.
   ```cpp
   class sealed final { ... };
   class derived : sealed {};   /* error: cannot derive from final class */
   ```

2. **On a virtual function** — forbids further overriding (step 27).

This task is about (1). It's neither a true keyword (you can still
name a variable `final`) nor part of the type system as such — but
the compiler enforces it at the inheritance site.

The trait `std::is_final<T>` is part of C++14, not C++11. To probe
final-ness from inside C++11 code, compilers provide a builtin:
`__is_final(T)`, used in this task's tests.

## Two use cases worth knowing

- **Sealing a class with no virtual functions.** Tells readers (and
  the compiler) that this type isn't meant to be a base. The
  compiler can devirtualize aggressively in some cases.

- **Sealing a virtual function in a deep hierarchy** (step 27). Stops
  the chain of overrides at a point you've audited.

## Task

In `solution.hpp`, declare two classes inside `task26`:

```cpp
class sealed final
{
public:
    int value() const;
private:
    int value_ = 42;
};

class openable
{
public:
    int value() const;
private:
    int value_ = 7;
};
```

The two classes differ only in the `final` specifier. `tests.cpp`
uses the `__is_final` compiler builtin to check this at compile time
and runtime tests verify both accessors return the expected values.

If you try to derive from `sealed` in a snippet like
`class child : public sealed {};`, the compiler emits "cannot derive
from final base". The tests don't include that line — it would
break the build. But it's the property `final` is designed to give
you.

## Run

```sh
./verify.sh
```

## Hints

- `final` goes immediately after the class name in the declaration:
  ```cpp
  class sealed final { ... };
  ```
  Not before `class`, not after the base-clause (if there is one),
  not after the closing `}`. Just after the name.

- `__is_final(T)` is a compiler builtin available on gcc and clang.
  It is not in the C++11 standard — but it underpins `std::is_final`
  in C++14. The tests use it because `std::is_final` would not
  compile under `-std=c++11`.

- Don't confuse `final` with the more familiar `const`. `const`
  prevents *modification*; `final` prevents *derivation*. Either or
  both can be on the same class.
