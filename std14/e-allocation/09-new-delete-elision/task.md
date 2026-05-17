# 9 — New/delete elision

> Spec: <https://en.cppreference.com/cpp/language/new#Allocation>

## What you learn

A long-standing limitation: `operator new` is a function call
with potential side effects (heap allocation, possible throw),
so the optimizer was traditionally not allowed to skip it,
even for short-lived allocations the compiler could see
locally.

C++14 codified that compilers **may elide** calls to
replaceable global allocation functions in specific
situations. The optimization existed in some implementations
before, but the standard wording made it explicitly legal
to:

- Skip an `operator new` call entirely (storage from
  automatic / static / other source instead).
- Fuse multiple allocations into one.

The classic motivating case is `std::make_shared<T>`: the
control block and `T` storage can be fused into a single
allocation, which is both faster and friendlier to the
allocator.

### The rules

The standard wording (C++14 [expr.new]/10, refined in C++17
[expr.new]/13) permits elision when:

1. **The new is balanced by a delete** that the compiler can
   prove will run. Typically: same function or a chain it
   can inline. Without seeing the matching delete, the
   compiler can't prove the lifetime and can't elide.

2. **The allocation's address does not escape** in a way that
   user code could observe. If you compare `p` against
   `nullptr`, store it in a global, pass it to a function
   the compiler can't see, or print it -- the elision could
   change observable behavior, so the compiler must keep the
   real allocation.

3. **The global allocator is not replaced** with one that has
   observable side effects. If a user supplies a tracking
   `::operator new` that logs every call, eliding would
   change what gets logged. The compiler must preserve
   observable behavior, so no elision.

The standard doesn't require elision -- it only permits it.
Different compilers and optimization levels make different
choices.

### Concrete scenarios

| Scenario                                                  | Elidable? | Why |
|-----------------------------------------------------------|-----------|-----|
| `new int(5); use(*p); delete p;` (same function)          | yes       | balanced, no escape |
| `new int(5); save_for_later(p);`                          | no        | escape |
| `std::make_shared<T>(...)` (control block + T)            | yes       | standard explicitly allows fusion |
| Program replaces `::operator new` with a logger           | no        | observable side effects |
| `new int(5);` with delete in another function the compiler can't inline | no | lifetime not provable |
| `new int[1000]` for a temporary buffer                    | maybe     | depends on escape analysis |

### `make_shared` fusion

The textbook win. Naive:

```cpp
std::shared_ptr<int> p(new int(5));   /* 2 allocations:
                                         1. new int       -- one alloc
                                         2. control block -- another alloc */
```

With `make_shared`:

```cpp
auto p = std::make_shared<int>(5);    /* 1 allocation:
                                         fused (control block + int)
                                         in a single contiguous block. */
```

The standard says `make_shared` "should" do this fusion. Most
implementations do. The result:
- One fewer allocator call.
- Better cache locality (control block and T adjacent in
  memory).
- One fewer deallocation.

The catch: the fused allocation must be retained until both
`shared_ptr` AND any outstanding `weak_ptr` are gone -- the
T's storage and the control block share a lifetime. If you
have a long-lived weak_ptr to a T that took 1MB of memory,
that 1MB is held until the last weak_ptr dies. With separate
allocations, the T's storage could be released earlier.

### Why this matters

For most user code: nothing changes. You write `new` and
`delete` and the compiler does what it can. The legalization
matters mainly for:

- Standard library implementers writing
  `make_shared`/`allocate_shared`.
- Optimizer authors who want to fold short-lived heap
  allocations into stack frames (the LLVM "promotion"
  pass).
- Code that benchmarks allocation patterns: the visible
  count of `operator new` calls may differ from the source
  count, even at -O0 sometimes.

### The optimization in practice

GCC and Clang at `-O2` will:

- Elide local `new`/`delete` pairs for trivial types when the
  pointer doesn't escape.
- Fuse `make_shared` allocations (the standard library code
  has explicit single-allocation paths).
- NOT elide if global `operator new` is replaced (they
  inspect the program for replacement declarations).

You can verify the second point: compile a program with and
without a custom `::operator new`, look at the allocation
counts in the compiled output. The fused form vanishes when
the operator is replaced.

### What you write here

This task doesn't write "real" elision-aware code -- the
compiler decides on its own. Instead, you classify a set of
hypothetical scenarios as elidable / not-elidable. The
function `may_elide(scenario)` returns the correct
classification for each enum case.

## Task

In `solution.hpp`, fill in `may_elide(scenario)` to return:

| Scenario                                  | Returns |
|-------------------------------------------|---------|
| `local_new_delete_paired`                 | `true`  |
| `pointer_escapes_to_unknown_callee`       | `false` |
| `make_shared_fused_allocation`            | `true`  |
| `replaced_global_operator_new`            | `false` |
| `new_without_matching_delete_in_scope`    | `false` |

The reference body is a short `||` expression.

## Run

```sh
./verify.sh
```

## Hints

- The function is `constexpr` -- the body should be a single
  return expression so it works in constant evaluation.

- The reference body:
  ```cpp
  return s == scenario::local_new_delete_paired
         || s == scenario::make_shared_fused_allocation;
  ```

- Both elidable cases share the same logical reason: the
  compiler has full visibility into the allocation's
  lifetime and the address doesn't escape (or the standard
  explicitly permits the fusion).

- The three non-elidable cases each fail for a different
  reason: pointer escapes (can't elide without breaking
  observers), replaced operator new (observable side
  effects), or no visible delete (no lifetime proof).

- The task is mostly conceptual -- the heavy lifting is the
  explanation in this task.md and in the comments in
  solution.hpp. The classifier just lets us write tests
  against the categorization.

- Tests verify: each scenario's expected classification and
  that the function is `constexpr` (usable in a constant
  expression).

- In production code, you don't need to "trigger" elision
  -- the compiler decides. If you care about allocation
  count, prefer `make_shared` over `shared_ptr<T>(new T)`,
  and avoid escaping local pointers when you can.
