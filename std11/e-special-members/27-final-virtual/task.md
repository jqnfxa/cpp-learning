# 27 — `final` on virtual functions

> Spec: <https://en.cppreference.com/cpp/language/final>

## What you learn

`final` has a second placement: on a virtual function. The function
itself can still be called; subclasses just can't *override* it
further.

```cpp
class base
{
public:
    virtual int rate() const { return 1; }
};

class middle : public base
{
public:
    int rate() const override final { return 2; }
    /*                       ^^^^^^^^^ */
    /*                       middle's override is the last word */
};

class leaf : public middle
{
public:
    /* int rate() const override { return 3; }   -- compile error */
};
```

The compiler error you'd get from the commented-out `leaf::rate` is
"overriding final function". The leaf can still *use* `rate()`; it
just can't replace it.

## When does this matter?

- **Locking down a hierarchy.** Three classes deep, you decide
  that further behavior changes for some method would be a design
  mistake. `final` stops the chain at that point.
- **Devirtualization.** When the compiler sees a `final` virtual
  call through a static-typed pointer, it can sometimes replace the
  vtable lookup with a direct call. Modest performance win.

You can combine `override` and `final` in either order:
`override final` or `final override`. The first is more common.

## Task

In `solution.hpp`, declare a 3-level hierarchy inside `task27`:

```cpp
class base
{
public:
    virtual ~base() = default;
    virtual int rate() const;     /* return 1 */
};

class middle : public base
{
public:
    int rate() const override final;   /* return 2 */
};

class leaf : public middle
{
public:
    int kind() const;     /* return 3; cannot override rate() */
};
```

Implement the three `rate()` / `kind()` accessors with the values in
the table.

`tests.cpp` covers:
- `base *p = &m; p->rate()` (virtual dispatch through base).
- `middle *p = &l; p->rate()` (leaf inherits middle's `rate`).
- `leaf::kind()` is its own thing.

There's no test for "leaf tried to override rate and got blocked" —
that would have to be a comment, since the offending code wouldn't
build. The structural test is "leaf::rate() returns 2 (inherited
from middle)".

## Run

```sh
./verify.sh
```

## Hints

- Order: `override final` (one space between, but `final override`
  also compiles).

- The base's `rate` is virtual; the middle's is `virtual` implicitly
  (inherited virtuality). You only need to write `virtual` on
  `base::rate`; on `middle::rate` it's optional and arguably noisy.

- Don't add `virtual` to `middle::rate`: it's still virtual (you'd
  just be repeating yourself), and writing `virtual ... override
  final` reads like the keyword soup the convention discourages.

- A virtual destructor in `base` is necessary if you ever delete a
  derived through a base pointer. The tests don't actually delete
  via base pointer, but the destructor stays as defensive practice
  for any class meant as a polymorphic base.
