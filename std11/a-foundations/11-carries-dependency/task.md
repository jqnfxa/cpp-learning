# 11 — Attributes: `[[carries_dependency]]`

> Spec: <https://en.cppreference.com/cpp/language/attributes/carries_dependency>

## Heads-up: this one is read-mostly

Unlike the rest of section A, this task is intentionally light on
code. `[[carries_dependency]]` is one of C++11's most obscure
attributes, tightly coupled to `std::memory_order_consume`, which
itself has effectively been disowned by every mainstream compiler.
The point of the step is *exposure to the syntax* — knowing it
exists when you see it in someone else's code — not muscle memory.

You'll add the attribute to two function declarations, implement
trivial bodies, and read the explanation below.

## What you learn

### The setup: weakly ordered hardware

On strongly ordered hardware (x86), a chain of pointer dereferences
is naturally ordered: if a thread publishes a pointer with a store,
and another thread reads that pointer and dereferences it, the
dereference cannot observe stale memory. The CPU enforces this.

On weakly ordered hardware (ARMv7, PowerPC), this is not free. The
reading CPU can speculatively reorder the dereference *before* the
pointer load, producing values from before the pointer was published.
To prevent that, the reader has to either:
- Use an acquire fence on the pointer load (`memory_order_acquire`), which
  is a full-pipeline barrier — expensive, and
- Use a *dependency chain*. The hardware respects data dependencies
  for free: if value `B` is computed from `A`, the read of `B` cannot
  be reordered before the read of `A` on these architectures. So
  loading the pointer and then dereferencing it through that pointer
  carries a dependency — and you don't need a fence.

The C++ memory model captured this with `memory_order_consume`. A
consume-load establishes a dependency chain; subsequent operations
that are *carried by* the chain inherit the synchronization without
the cost of acquire.

### Where the attribute fits

The compiler can only preserve a dependency chain if it knows the
chain exists. Functions hide the chain: if you pass a
just-consume-loaded pointer through a function boundary, the compiler
loses the dependency information unless it's told to keep it.

```cpp
/* Return value carries a dependency chain to the caller. */
[[carries_dependency]] const int *publish_or_null();

/* Parameter is a dependency chain into the function;
 * the body may carry it into further loads. */
void consume([[carries_dependency]] const int *p);
```

Without the attribute, the compiler may emit an acquire fence at the
function boundary to be safe. With it, the compiler trusts that the
caller will pass a dependency chain through.

### Why it's effectively vestigial in 2024

[P0371](https://wg21.link/P0371) (and follow-ups) document that
implementing `memory_order_consume` correctly is so hard that GCC and
Clang **treat it as `memory_order_acquire`** instead. The intended
optimization doesn't actually fire. `[[carries_dependency]]` is
therefore mostly a hint to readers, not to the compiler. The
attribute is preserved in the standard so that code using it remains
well-formed; future work may revive consume semantics.

In short: know it exists, recognize it in other people's code, and
default to `memory_order_acquire` when you actually need acquire
semantics.

## Task

In `solution.hpp`, attach `[[carries_dependency]]` to two function
declarations and implement them:

| Function                     | Where the attribute goes | Body                  |
|------------------------------|---------------------------|-----------------------|
| `get_token()`                | on the function (return value carries) | return a pointer to a `static const int` with value `42` |
| `consume_token(const int *p)` | on the parameter `p` (parameter consumes) | return `*p` |

The tests don't (and can't) observe the attribute directly — there is
no `std::has_attribute<...>` trait. They only check the bodies behave
correctly. The point is the syntax, plus the explanation above.

## Run

```sh
./verify.sh
```

## Hints

- Return-value placement (before the return type):
  ```cpp
  [[carries_dependency]] const int *get_token();
  ```
- Parameter placement (between the type and the name):
  ```cpp
  int consume_token([[carries_dependency]] const int *p);
  ```
- For the body of `get_token`, a function-local `static const int`
  has a stable address you can return a pointer to:
  ```cpp
  static const int value = 42;
  return &value;
  ```
- For `consume_token`, just `return *p;`.
