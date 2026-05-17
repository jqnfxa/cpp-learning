# 17 — `noexcept` is part of the type

> Spec: <https://en.cppreference.com/cpp/language/noexcept_spec>

## What you learn

In C++17, `noexcept` became part of the function type.
`int(*)(int)` and `int(*)(int) noexcept` are now DIFFERENT
types. Conversion rules:

- `noexcept fp` → `non-noexcept fp`: **OK** (you can weaken
  the guarantee).
- `non-noexcept fp` → `noexcept fp`: **error** (you can't
  strengthen).

This catches a real bug: assigning a possibly-throwing
function to a slot that promised noexcept would let
exceptions escape and bypass the noexcept guarantee. C++17
makes it a compile error.

## Task

Implement two helpers that call `squared_noexcept` through
two function-pointer types. The reference shows both
directions of pointer-type assignment.

## Run

```sh
./verify.sh
```

## Hints

- `int (*f)(int) = &squared_noexcept;` -- assigning a
  noexcept function to a non-noexcept fp. Legal (weakens).

- `int (*f)(int) noexcept = &squared_noexcept;` -- matching
  noexcept types. Legal.

- The reverse (`int (*f)(int) noexcept = &squared_throws;`)
  would NOT compile -- can't strengthen.

- `std::is_convertible_v<fp_noexcept, fp_throw>` is true;
  the reverse is false. Tests pin both directions.

- Function-overload-set behaviour also changed: you can
  overload on `noexcept`-ness in C++17 (in principle; very
  rarely useful).
