# 6 — Init-capture (capture by move / by computed expression)

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

C++11 lambdas had two capture forms:

- `[x]` -- capture `x` by value (copy).
- `[&x]` -- capture `x` by reference.

There was no way to:

- Capture-by-move (needed for `unique_ptr` and other
  move-only types).
- Initialise the captured member from an expression that
  isn't a name in scope.
- Rename the captured member.

C++14 added **init-capture**: `[name = expression]`. The
captured member is named `name` (in the lambda's body) and
initialised from `expression` once, at lambda construction
time:

```cpp
auto lam = [count = initial]() mutable { return count++; };
```

`count` is a member of the closure type; `initial` (some
variable in scope at the lambda site) is the initialiser.
After construction, the closure holds its own `count` and
no longer refers to `initial`.

### The big three uses

1. **Move-capture**:
   ```cpp
   auto p = std::make_unique<int>(42);
   auto lam = [p = std::move(p)]() { return *p; };
   /* lam owns the unique_ptr; the outer p is now empty */
   ```

2. **Computed-value capture**:
   ```cpp
   auto lam = [size = vec.size() * 2]() { return size; };
   /* the captured member is the COMPUTED value, not vec */
   ```

3. **Renaming**:
   ```cpp
   auto lam = [n = some_long_external_name]() { return n; };
   /* shorter name `n` is what's used in the body */
   ```

The right-hand side can be any expression; the type of the
captured member is deduced from the expression (like `auto`).

### Move-capture in detail

In C++11, you simulated move-capture with bind:

```cpp
auto lam = std::bind(
    [](std::unique_ptr<int> &p) { return *p; },
    std::move(p));
```

This worked, but bind has overhead, awkward syntax, and
doesn't compose well with `std::function` and STL
algorithms. C++14's init-capture is the clean replacement:

```cpp
auto lam = [p = std::move(p)]() { return *p; };
```

`p` (inside the lambda) is a `std::unique_ptr<int>` member of
the closure type. The outer `p` (the local that was moved
from) is left in the moved-from state.

### Mutable, mutability

A lambda's captured members are `const` by default -- the
call operator is `const`. To mutate captured state, mark the
lambda `mutable`:

```cpp
auto lam = [count = initial]() mutable { return count++; };
                                ^^^^^^^
```

Without `mutable`, `count++` fails to compile (can't
increment a const int).

For move-captured `unique_ptr<T>`, you usually don't need
`mutable` for reading (`*p`) but DO need it for resetting
(`p.reset()`).

### Type deduction

The captured member's type is deduced from the initialiser
using `auto` rules (so references and top-level cv strip).
If you want a reference-typed capture, write:

```cpp
auto lam = [&r = some_obj]() { ... };     /* r is a reference */
```

The `&` here is part of the capture syntax, not part of the
expression. It binds `r` as a reference to `some_obj`.

### Restrictions

- The right-hand side must be a valid expression in the
  enclosing scope. You can't access lambda-body names (the
  lambda doesn't exist yet).
- Move-captured members are owned by the closure -- they live
  as long as the closure does.
- If the closure is copied, the captured members are copied
  (which fails for move-only types like `unique_ptr` -- such
  closures are themselves move-only).

### The std::function trap

`std::function<...>` (C++14) requires its target to be
**copyable**. A lambda that move-captures a `unique_ptr` is
move-only, so wrapping it in `std::function` is a compile
error:

```cpp
std::function<int()> f = [p = std::make_unique<int>(42)](){ return *p; };
/* error: closure is not copyable */
```

Workarounds:

1. Use `std::shared_ptr` instead: it's copyable, and
   init-capture works the same way.
2. Use a custom callable wrapper (or C++23
   `std::move_only_function`).

The reference implementation in `hold_unique` uses
workaround (1): convert the `unique_ptr` to a `shared_ptr`
and move-capture the shared one. The init-capture machinery
is exactly the same; only the type changes.

## Task

In `solution.hpp`, fill in the bodies of three functions:

1. `make_counter(int initial)`: return a lambda that yields
   `initial, initial+1, initial+2, ...`. Use init-capture
   `[count = initial]` and `mutable`.

2. `make_growable(std::string initial)`: return a lambda that
   appends `"!"` to a captured string and returns the new
   size. Use **move-capture** `[s = std::move(initial)]` and
   `mutable`.

3. `hold_unique(std::unique_ptr<int> p)`: return a
   `std::function<int()>` that yields `*p`. Convert the
   `unique_ptr` to a `std::shared_ptr` first (since
   `std::function` is copy-only), then move-capture the
   shared_ptr.

```cpp
namespace task06
{
    auto make_counter(int initial);
    auto make_growable(std::string initial);
    std::function<int()> hold_unique(std::unique_ptr<int> p);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The bodies are short:
  ```cpp
  return [count = initial]() mutable { return count++; };

  return [s = std::move(initial)]() mutable {
      s += "!";
      return s.size();
  };

  auto sp = std::shared_ptr<int>(std::move(p));
  return [sp = std::move(sp)]() { return *sp; };
  ```

- `mutable` is required on `make_counter` (because
  `count++` mutates) and `make_growable` (because `s += "!"`
  mutates).

- `hold_unique` does NOT need `mutable` -- the body only
  reads `*sp`.

- The `std::shared_ptr<int>(std::move(p))` step is the
  std::function trap workaround. shared_ptr is copyable, so
  the closure is copyable, so std::function can wrap it.

- `make_counter` returns `auto` -- the closure type. Don't
  worry about naming it.

- `make_growable` also returns `auto`. The captured string is
  owned by the lambda's storage; the caller's local is
  empty after the function returns.

- Tests verify: counter starts at the right value, counter
  increments across calls, multiple counters are independent,
  growable appends correctly, growable persists state across
  calls, hold_unique returns the pointed value and can be
  called repeatedly.

- The "growable_empty_initial" test is a coincidence: stub
  and reference both produce size 1 for input `""`. It's a
  sanity test, not a stub-detector.

- The "hold_unique_*" tests all pass on the stub because the
  stub eagerly dereferences `p` and stores the int. The
  pedagogical point (init-capture by move) is lost in the
  stub, but the by-value tests can't distinguish. The
  pedagogical proof is in the CODE -- the reference uses
  init-capture, the stub doesn't.
