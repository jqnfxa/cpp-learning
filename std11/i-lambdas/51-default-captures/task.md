# 51 — Default captures `[=]` and `[&]`

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

Listing each captured variable in the capture list works (`[a, b,
c]`), but for a lambda that touches a lot of locals it gets
verbose. The **default capture** lets you abbreviate:

| Capture | Meaning                                            |
|---------|-----------------------------------------------------|
| `[=]`   | every used local is captured **by value** (snapshot)|
| `[&]`   | every used local is captured **by reference** (live) |
| `[=, &x]` | by value default, except `x` is by reference   |
| `[&, x]`  | by reference default, except `x` is by value   |

The compiler only captures what the lambda actually uses — not
the entire enclosing scope. If you write `[=]` and only mention
`a` inside, only `a` is captured.

## Trade-offs

- **`[=]`** is the "safer" default for lambdas you store or
  return. Each variable becomes a snapshot, no lifetime
  surprises.

- **`[&]`** is right for short-lived lambdas that need to mutate
  or observe live state, e.g. inside a `std::for_each` body. Avoid
  for lambdas you escape (return, store) past the enclosing scope.

- The mixed forms (`[=, &x]` and `[&, x]`) are the "I want most of
  this captured one way and just this one differently" tool. Use
  sparingly; if the override list is long, just spell out all
  captures.

## Task

In `solution.hpp`, implement two functions that each declare three
local variables, build a lambda using a default-capture form, then
*modify* the locals before invoking the lambda. The return value
shows whether the lambda saw the snapshot or the live values:

```cpp
/* Use [=] to capture all three locals by value, then modify them
 * before invoking. Should return the SNAPSHOT sum (60). */
int snapshot_sum();

/* Use [&] to capture all three locals by reference, then modify
 * them before invoking. Should return the LIVE sum (600). */
int live_sum();
```

Skeleton for both:

```cpp
int a = 10, b = 20, c = 30;
auto sum = /* [=] or [&] */ []() { return a + b + c; };
a = 100;
b = 200;
c = 300;
return sum();
```

| Function          | Returns | Why                            |
|-------------------|---------|--------------------------------|
| `snapshot_sum()`  | `60`    | `[=]` captured 10+20+30 = 60   |
| `live_sum()`      | `600`   | `[&]` sees 100+200+300 = 600   |

## Run

```sh
./verify.sh
```

## Hints

- `snapshot_sum`:
  ```cpp
  int a = 10, b = 20, c = 30;
  auto sum = [=]() { return a + b + c; };
  a = 100; b = 200; c = 300;
  return sum();
  ```

- `live_sum`: same body but `[&]` instead of `[=]`.

- The compiler captures *only what's referenced* inside the
  lambda. `[=]` doesn't snapshot the entire function-local
  universe; it snapshots `a`, `b`, `c` because those are what
  `sum` uses.

- A common style guideline: prefer **explicit** captures
  (`[a, b, c]` or `[&a, &b, &c]`) over defaults in production
  code. Default captures save typing but make code reviews
  harder: a reader has to scan the lambda body to know what's
  captured. For tightly-scoped helpers (like in this task) the
  default form is fine.
