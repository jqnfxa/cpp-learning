# 8 — Aggregates with default member initializers

> Spec: <https://en.cppreference.com/cpp/language/aggregate_initialization>

## What you learn

In C++11, a class was an **aggregate** (eligible for brace-
init) only if it had:

- No user-declared constructors.
- No private/protected non-static data members.
- No virtual functions or base classes.
- **No default member initializers.**

That last rule made the obvious thing not work:

```cpp
/* C++11: this is NOT an aggregate, so brace-init fails */
struct point {
    int x = 0;
    int y = 0;
};
point p{1, 2};   /* C++11 error: braces don't work on non-aggregates */
```

You had to choose: either get brace-init (no defaults) or
get defaults (no brace-init). The fix in C++11 was to write
a constructor or omit the defaults.

C++14 removed that restriction. **Default member
initializers no longer disqualify a class from being an
aggregate.** You get both:

```cpp
/* C++14: aggregate AND has defaults */
struct point {
    int x = 0;
    int y = 0;
};

point a{};       /* x=0, y=0    -- from defaults  */
point b{1};      /* x=1, y=0    -- y from default */
point c{1, 2};   /* x=1, y=2    -- both explicit  */
point d;         /* x=0, y=0    -- default ctor   */
```

### The interplay: brace overrides default

The brace list and the defaults interact predictably:

- If you brace-init with N args, the first N members get the
  brace values.
- The remaining members fall through to their declared
  defaults.
- If there are no defaults for the trailing members, they're
  **value-initialized** (zero-initialized for ints, etc.).

```cpp
struct s {
    int a;          /* no default */
    int b = 10;
    int c = 20;
};

s x{1, 2, 3};       /* a=1, b=2, c=3 */
s y{1, 2};          /* a=1, b=2, c=20 */
s z{1};             /* a=1, b=10, c=20 */
s w{};              /* a=0 (value-init), b=10, c=20 */
```

The "trailing-defaults" behavior is **why** C++14 lifted the
restriction: with C++11's no-default rule, you had to write
out a constructor for every shape of "some explicit, some
default."

### Default ctor vs `{}`

`point p{}` and `point p` produce the same result for
default-initializer aggregates:

- `point p` -- default-initialization. Members with defaults
  use those; members without defaults are
  **default-initialized** (POD types: indeterminate values!).
- `point p{}` -- value-initialization via empty braces.
  Members with defaults use those; members without are
  **value-initialized** (zero for arithmetic types).

For aggregates where every member has a default, the two are
indistinguishable. For aggregates with a mix, `p{}` is safer
because it zero-fills the no-default members.

### How brace-init reaches members

Aggregate brace-init does **memberwise direct-init** in
declaration order. Each brace element matches the next
member; defaults supply values for members not in the brace
list. No constructor call (the class has none); just storage
laid out and members initialized one by one.

This works for nested aggregates too:

```cpp
struct outer { point p; int z = 0; };
outer o{{1, 2}, 3};     /* p={1,2}, z=3 */
outer o2{};             /* p={0,0}, z=0 */
```

### Brace-init narrowing

Aggregate brace-init STILL forbids narrowing conversions:

```cpp
struct s { int n; };
s x{1.5};       /* error: narrowing double -> int */
```

Defaults don't help here -- the brace list's value is checked
against the member's declared type. The same narrowing rule
applies as for any other brace-init.

### Doesn't apply to "no longer an aggregate" reasons

The other C++11 aggregate disqualifiers still apply in
C++14:

- User-declared ctors -> not aggregate (even in C++14).
- Private/protected members -> not aggregate.
- Virtual functions -> not aggregate.
- Bases (in C++14; C++17 relaxed this to allow public bases).

Only the **default-member-initializer** rule was lifted.

### C++17 update: public bases allowed

C++17 went further: aggregates may have public bases, and
brace-init can fill base subobjects too. C++14 doesn't have
that; for us, aggregates are still "no inheritance."

### Designated initializers (C++20)

C++20 adds `point p{.x = 1, .y = 2};` -- by-name init for
aggregates. Pre-C++20, you can only use positional brace-init.
For our C++14 setup, positional is what we have.

## Task

In `solution.hpp`, set the default values for two aggregates:

```cpp
struct point {
    int x = 0;
    int y = 0;
};

struct user {
    std::string name = "anonymous";
    int age = 0;
    bool active = true;
};
```

The defaults must be such that `point{}` is `{0, 0}` and
`user{}` is `{"anonymous", 0, true}`. Brace-init with fewer
than the full number of args falls through to the
declarations for the remaining members.

## Run

```sh
./verify.sh
```

## Hints

- The members are just declarations -- write
  `int x = 0;`, `int y = 0;` for `point`, and similarly for
  `user`.

- Do NOT add a constructor. The classes must remain
  aggregates (no user-declared ctors).

- `point p{}` invokes value-init: members with defaults take
  those values, anything else is zeroed. Since both have
  defaults, it just runs the defaults.

- `point p;` is default-init: in C++14, for an aggregate with
  in-class member initializers, this also runs the defaults.

- `point p{1}` is partial brace-init: x=1 (explicit),
  y=0 (default).

- `user u{}` -> `{"anonymous", 0, true}`. Stub's WRONG
  default for `active` (false) is what the dedicated
  `user_active_default_is_true` test pins.

- `user u{"alice"}` -> name from brace, age and active from
  defaults. Same trailing-defaults pattern as `point{1}`.

- Tests verify: point {} / no-init / {x} / {x,y} / negative
  values, user {} / no-init / partial-brace / full-brace,
  and a specific check that `active` defaults to true.

- The brace-init does memberwise direct-init in declaration
  order; the defaults fill in the remaining members. Don't
  reorder the members or the brace-init meaning changes.

- C++17 added inline variables for inline aggregate
  definitions, and C++20 added designated initializers
  (`{.x = 1}`). Neither is needed here; standard C++14
  positional brace-init is the form being demonstrated.
