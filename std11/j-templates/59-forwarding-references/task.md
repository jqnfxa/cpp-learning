# 59 — Forwarding references in templates

> Spec: <https://en.cppreference.com/cpp/language/reference#Forwarding_references>

## What you learn

A **forwarding reference** (sometimes "universal reference") is a
`T &&` parameter where:

1. `T` is a **deduced** template parameter of the function, and
2. The parameter is spelled exactly `T &&` (not `const T &&`, not
   `std::vector<T> &&`).

When both conditions hold, the parameter's category-deduction
behaves specially: passing an lvalue makes `T` deduce as `T &`
(which then collapses `T && &` → `T &`); passing an rvalue makes
`T` deduce as `T` (parameter type stays `T &&`). This is exactly
the machinery you exercised in step 37 (reference collapsing) and
step 36 (perfect forwarding).

```cpp
template <class T>
void f(T &&arg);   /* forwarding reference */

int x = 0;
f(x);              /* T = int &,  arg has type int &  */
f(7);              /* T = int,    arg has type int && */
f(std::move(x));   /* T = int,    arg has type int && */
```

A `T &&` that is **not** a deduced template parameter is just an
rvalue reference, not a forwarding reference:

```cpp
void g(int &&arg);                    /* rvalue ref, NOT forwarding */
template <class T> void h(const T &&); /* rvalue ref, NOT forwarding */
template <class T> struct S { void m(T &&); };  /* T from the class,
                                                   NOT deduced -- rvalue ref */
```

The forwarding/rvalue distinction matters because forwarding refs
bind to both lvalues and rvalues; plain rvalue refs only to rvalues.

## Task

In `solution.hpp`, implement two function templates that use a
forwarding reference and report the category of the argument
that was passed:

```cpp
/* Returns "lvalue" if `arg` was passed as an lvalue, "rvalue"
 * if it was an rvalue (xvalue or prvalue). */
template <class T>
const char *category(T &&arg);

/* Returns the deduced T (encoded as a string). Useful for seeing
 * the type-deduction rule clearly. */
template <class T>
const char *deduced_kind(T &&arg);
```

| Call                              | `category(...)` | `deduced_kind(...)` |
|-----------------------------------|-----------------|---------------------|
| `int x = 0; category(x)`          | `"lvalue"`      | `"T_amp"`           |
| `category(42)`                    | `"rvalue"`      | `"T"`               |
| `category(std::move(x))`          | `"rvalue"`      | `"T"`               |
| `int &&rr = 1; category(rr)`      | `"lvalue"` (named T&& is lvalue) | `"T_amp"` |

`deduced_kind` reports what `T` *actually deduced as*:
- `"T_amp"`  → `T` deduced as a reference type (`int &`)
- `"T"`      → `T` deduced as a non-reference type (`int`)

These are the two cases that arise from forwarding-reference
deduction (per C++11 template argument deduction rules).

## Run

```sh
./verify.sh
```

## Hints

- `category` body:
  ```cpp
  return std::is_lvalue_reference<T>::value ? "lvalue" : "rvalue";
  ```
  After reference collapsing, `T` is `int &` for lvalue args and
  plain `int` for rvalue args. `is_lvalue_reference<T>` discriminates.

- `deduced_kind` body: same shape, different strings.
  ```cpp
  return std::is_reference<T>::value ? "T_amp" : "T";
  ```

- This step is closely related to steps 36 (perfect forwarding)
  and 37 (reference collapsing). The new framing is "T && is a
  forwarding reference when T is a deduced template parameter" --
  the previous steps were about *what happens after deduction*;
  this one is about *recognizing the forwarding-reference shape*.

- A common trap: `template <class T> void f(std::vector<T> &&)`
  is NOT a forwarding reference. `T` is deduced, but the
  parameter type is `vector<T> &&`, not `T &&`. It only binds to
  rvalues.
