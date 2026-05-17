# 5 — Guaranteed copy elision

> Spec: <https://en.cppreference.com/cpp/language/copy_elision>

## What you learn

Pre-C++17, **copy elision** was an *optimization the compiler
WAS ALLOWED TO PERFORM*: the compiler could skip a copy/move
when returning a temporary. But it didn't have to. And the
return-by-value still required the type to be at least
movable (for the standard to be conformant on -O0).

C++17 promoted certain forms of copy elision to **mandatory**.
For:

- Returning a **prvalue** (`return T(args);`) from a function.
- Copy-initializing a variable from a prvalue (`T x = f();`).

The compiler **MUST NOT** call any copy/move constructor. The
returned object is constructed directly in the caller's
destination storage.

This unlocks **return-by-value of non-movable types**:

```cpp
struct mutex_holder {
    mutex_holder() = default;
    mutex_holder(const mutex_holder&) = delete;
    mutex_holder(mutex_holder&&) = delete;
};

mutex_holder make_holder() {
    return mutex_holder{};      /* legal in C++17 -- no move needed */
}

mutex_holder m = make_holder(); /* legal -- elided */
```

Before C++17, both lines would fail to compile (`delete`d
move constructor).

### What "prvalue" means here

In C++17, value categories changed:

- **prvalue**: pure rvalue. The result of `T()`, `T{args}`,
  return-of-literal, etc.
- **xvalue**: expiring rvalue. The result of `std::move(x)`,
  `static_cast<T&&>(x)`.
- **glvalue**: anything that's named (lvalue or xvalue).

Mandatory elision applies to **prvalues only**. If you write:

```cpp
return std::move(x);      /* x is named -> xvalue -- NOT elided */
```

You force a move. The optimizer might still elide it in
practice, but the standard doesn't guarantee it. To benefit
from guaranteed elision, return PRVALUE expressions.

### Two forms covered

**Direct return**:

```cpp
T make() { return T(args); }   /* prvalue: elision guaranteed */
```

vs

```cpp
T make() {                     /* xvalue: elision not guaranteed */
    T t(args);
    return std::move(t);
}
```

**Direct initialization from prvalue**:

```cpp
T x = make();                  /* RHS is a prvalue -- elision guaranteed */
```

vs

```cpp
T t = ...;
T x = std::move(t);            /* RHS is an xvalue -- move happens */
```

### NRVO (Named Return Value Optimization)

When you have a named local variable returned:

```cpp
T make() {
    T t(args);
    return t;             /* named return -- NOT guaranteed elision */
}
```

This is called NRVO. It's NOT mandatory in C++17 (only the
prvalue form is). The compiler USUALLY elides anyway, but
the type still needs to be movable -- the standard reserves
the right to call the move ctor here.

To get guaranteed elision with NRVO, you'd need C++20 (which
extended elision rules further). For now, C++17 only
guarantees elision for prvalue return.

### Why the change matters

Three concrete wins:

1. **Non-movable factories**: as shown above. Types that
   guard shared state (mutexes, files, hardware locks) can
   now be returned by value.

2. **Composition**: factories can be chained without
   move-cost concern:
   ```cpp
   T x = inner(outer(some_args));   /* zero moves in C++17 */
   ```

3. **Code clarity**: you don't need to add `std::move` for
   "performance" reasons when returning a prvalue. The
   compiler does the right thing automatically.

### What's still NOT guaranteed

- NRVO (named-variable return) -- the compiler typically
  elides, but the type must remain movable.
- Pass-by-value parameter elision -- the type must be
  movable.
- Throwing -- when an exception leaves a scope, the in-
  flight return is undone.

For C++17 strict guarantees, stick to prvalue returns and
prvalue initializations.

### How to verify elision in tests

We test elision by **counting moves and copies** via a type
with instrumented constructors:

```cpp
struct trace { static int moves, copies; ... };

trace make_it() { return trace{}; }   /* prvalue */
trace t = make_it();
EXPECT_EQ(trace::moves, 0);
EXPECT_EQ(trace::copies, 0);
```

If your code returns a prvalue, the counts are zero. Any
non-zero count means you've forced a move or copy somewhere.

The complementary "explicit move does count" test verifies
the counter is working: `t = std::move(other)` IS counted.

### The `T = make()` requirement

Guaranteed elision needs the destination to be initialized
from the prvalue:

```cpp
T x = make();           /* OK: x initialized from prvalue */
T x(make());            /* OK */
T x{make()};            /* OK */
```

This is "copy-initialization from a prvalue" -- the
mandatory-elision case.

```cpp
T x;
x = make();             /* assignment: move-assignment happens */
```

The last is **assignment**, not initialization. The move-
assignment operator IS called (which must exist).

## Task

In `solution.hpp`, fill in two factory functions:

```cpp
inline lock_handle make_lock(int id) {
    return lock_handle(id);
}

inline trace_type make_traced(int v) {
    return trace_type(v);
}
```

Both return **prvalues** (`T(args)`), which C++17 guarantees
to construct directly in the caller's destination.

## Run

```sh
./verify.sh
```

## Hints

- The TODO in `make_lock`: return `lock_handle(id)`. The
  stub returns `lock_handle(-1)` (wrong id).

- The TODO in `make_traced`: return `trace_type(v)` directly.
  The stub creates a local and returns `std::move(t)` --
  forcing one move that breaks the elision contract.

- `lock_handle` has copy/move both `delete`d. The stub's
  return-by-prvalue WORKS under C++17 because no move is
  needed. Pre-C++17 this entire function wouldn't compile.

- The `make_traced_no_moves` test is the elision-detection
  test: after a prvalue return, the trace_type's move count
  and copy count should both be 0.

- `std::move` produces an XVALUE, not a prvalue. Guaranteed
  elision does NOT apply -- a move is performed. This is
  why the stub's count is 1 (one move).

- For named-return-value optimization (NRVO),
  `return some_local;`, C++17 does NOT guarantee elision.
  The optimizer usually elides anyway, but the type must
  still be movable. For non-movable types, return the
  prvalue form.

- The "explicit_move_does_count" test verifies that the
  trace_type's counters work: `std::move(x)` does register a
  move. If this test passes but `make_traced_no_moves` also
  passes, that's the proof of elision.

- Three guarantees for guaranteed elision: (1) return
  expression is a prvalue, (2) destination is initialized
  from the prvalue, (3) the type doesn't matter (movable
  or not, both work).

- The C++17 wording is in [class.copy.elision]/3 ("In the
  following circumstances, the implementation is required to
  omit the copy/move construction of a class object").
