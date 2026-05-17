# 2 — `constexpr` member functions are no longer implicitly `const`

> Spec: <https://en.cppreference.com/cpp/language/constexpr>

## What you learn

C++11 made every `constexpr` member function **implicitly
`const`**. So you couldn't write a `constexpr` setter -- the
function signature `constexpr void set_x(int)` was silently
treated as `constexpr void set_x(int) const`, and the body
`x = v;` failed to compile.

C++14 dropped that implicit-const. A `constexpr` member
function is now exactly as `const` as you wrote it. If you
omit `const`, it's non-const and can mutate the object:

```cpp
struct point
{
    int x, y;

    constexpr point(int x_, int y_) : x(x_), y(y_) {}

    constexpr void set_x(int v) { x = v; }   /* legal in C++14 */
    constexpr void set_y(int v) { y = v; }
};
```

Combined with relaxed constexpr bodies (step 1), this means
you can write a normal mutable C++ class and have every
operation usable at compile time.

### Why this matters

Without constexpr setters, the C++11 idiom for building a
literal type was:

```cpp
constexpr point p{3, 4};
constexpr point q = p.with_x(99);     /* returns a NEW point */
```

Every "change" was a returns-new-object operation -- not a
mutation. That worked, but for complex types (matrices,
buffers, tries) you wrote a lot of `with_*` helpers and
relied on copy elision.

C++14 lets you write the imperative form:

```cpp
constexpr point make_point(int x, int y)
{
    point p;
    p.set_x(x);     /* mutation in constexpr context */
    p.set_y(y);
    return p;
}
```

Same compile-time evaluability, much more natural code.

### What "non-const" buys you in a literal type

A `constexpr` setter can only be called on a non-const
object. So:

```cpp
constexpr point p1{0, 0};
p1.set_x(5);                    /* error: p1 is a constexpr OBJECT, const */

constexpr point p2 = []{
    point local{0, 0};
    local.set_x(5);             /* OK: local is non-const */
    return local;
}();
```

The key insight: `constexpr` on a VARIABLE makes it const at
the named-variable level, but TEMPORARY constexpr objects
during evaluation (locals inside another constexpr function)
can be mutated. The compiler tracks state through these local
mutations and produces the final value as the constexpr
result.

This is what makes "build a literal type via setters" work:
the building happens in a constexpr function frame, where the
local object is mutable, even though the eventual variable
holding the result is const.

### What about reads?

Reads (`get_x() const`) stay `const`. You explicitly mark
them so they can be called on both const and non-const
objects:

```cpp
constexpr int get_x() const noexcept { return x; }
```

The const here is YOUR const, not the language's implicit
const. Without it, you couldn't call `p.get_x()` on a
`const point p` -- so for read-only members, you ALWAYS
write const explicitly.

### Why this didn't get noticed in C++11

In C++11, `constexpr` was niche -- mainly used for compile-
time constants and very small functions. The fact that you
couldn't have mutating constexpr members rarely came up. With
C++14's relaxation (loops, locals), people started writing
"real" code in constexpr context, and the implicit-const
rule was the obvious blocker. C++14 fixed it.

### What if you DO want const?

Then write `const`. It's just a normal C++ member function;
the `constexpr` doesn't affect the const-qualification
either way:

```cpp
constexpr int distance_to_origin() const noexcept    /* C++14 */
{
    return x * x + y * y;
}
```

So C++14's change is "make const explicit," not "remove
const." Read-only functions get `const`, writers don't.

### What's still implicit?

Two related cases:

1. **`constexpr` non-member functions**: never had implicit
   const (they have no `this`). No change.
2. **`constexpr` constructors**: never had implicit const
   either (the object isn't fully constructed yet, so const-
   qualification doesn't apply). No change.

The change is specifically about `constexpr` non-static
member functions on already-constructed objects.

## Task

In `solution.hpp`, fill in the bodies of `point::set_x` and
`point::set_y`. They're declared `constexpr void set_*(int)`
(no `const`), so they can mutate `x` and `y`. The bodies
should be the obvious `x = v;` and `y = v;`.

```cpp
struct point
{
    int x, y;
    constexpr void set_x(int v) noexcept;
    constexpr void set_y(int v) noexcept;
};

constexpr point make_point(int x, int y) noexcept;
```

`make_point` is already wired up: it creates a local `point`,
calls the setters, and returns. With working setters, this
becomes the canonical "build a literal at compile time via
setters" idiom.

## Run

```sh
./verify.sh
```

## Hints

- The bodies are literally one line each: `x = v;` and `y = v;`.

- Do NOT add `const` to the setters -- that would re-introduce
  the C++11 problem and the body would fail to compile.

- Keep `constexpr` on both setters. Without it, calling them
  inside `make_point` (which IS constexpr) would force
  `make_point`'s compile-time evaluation to fail.

- The `make_point` body uses the setters in a constexpr
  context. The compiler tracks the local `point p`'s state
  across the two assignments and returns the final value.

- Tests verify: default ctor zeros, explicit ctor assigns,
  set_x mutates x only (and set_y mutates y only), multiple
  setters compose, make_point works at runtime, make_point
  works at COMPILE time (the `constexpr point p = ...` lines
  force this), zero/negative values, getters work on const
  objects.

- The `compile_time_*` tests are the C++14-only behavior:
  they declare `constexpr point p = make_point(...)`. If your
  setters aren't `constexpr` or aren't non-const, those lines
  fail to compile. The reference implementation passes them
  trivially.
