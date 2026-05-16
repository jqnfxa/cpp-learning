# 30 — Rvalue references (`T &&`)

> Spec: <https://en.cppreference.com/cpp/language/reference>

## What you learn

C++11 added a second flavor of reference: the **rvalue reference**,
written `T &&`. Where `T &` binds only to lvalues, `T &&` binds only
to rvalues (xvalues and prvalues — see step 29).

You don't usually use `T &&` to declare a *variable* — that's a
specialized thing. You use it to declare a *parameter*, so the
function can accept rvalues distinctly from lvalues:

```cpp
void f(int &x);    /* accepts lvalues */
void f(int &&x);   /* accepts rvalues */
```

Overload resolution picks between them based on the value category
of the argument expression:

```cpp
int a = 0;
f(a);              /* f(int &) -- a is lvalue */
f(7);              /* f(int &&) -- 7 is prvalue */
f(std::move(a));   /* f(int &&) -- std::move produces xvalue */

int &&rr = 0;
f(rr);             /* f(int &) -- the *name* rr is an lvalue (gotcha!) */
f(std::move(rr));  /* f(int &&) */
```

The gotcha (also from step 29): a named `T &&` variable is itself an
lvalue when used. You need `std::move` to convert it back to an
rvalue for forwarding. This is the bridge between value categories
and `std::move`, which gets its own step next.

## Task

In `solution.hpp`, declare two overloads inside `task30`:

```cpp
int f(int &x);     /* return 1 -- "lvalue overload" */
int f(int &&x);    /* return 2 -- "rvalue overload" */
```

The function body just returns the sentinel — tests look at the
return value to confirm overload resolution. Tests cover:

| Call                              | Expected return |
|-----------------------------------|-----------------|
| `f(a)` (where `a` is an `int`)    | `1`             |
| `f(7)` (literal)                  | `2`             |
| `f(std::move(a))`                 | `2`             |
| `f(r)` (where `r` is `int &`)     | `1`             |
| `f(rr)` (where `rr` is `int &&`)  | `1` (gotcha)    |
| `f(std::move(rr))`                | `2`             |

## Run

```sh
./verify.sh
```

## Hints

- Two overloads, two one-line bodies. The point is the *signatures*
  picking up different argument categories, not the implementations.

- `std::move(x)` is in `<utility>`. It's a cast — it does not move
  anything by itself. It just changes how the expression is
  classified, which changes overload selection.

- The `f(rr)` test is the test you want to pay attention to. The
  named rvalue-reference variable `rr` looks like an rvalue from the
  *type* perspective, but expressed as `rr` it's an lvalue. Without
  `std::move`, it binds to the `f(int &)` overload. This is the
  whole reason `std::move` exists.
