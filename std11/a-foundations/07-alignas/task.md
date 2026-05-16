# 07 — `alignas`

> Spec: <https://en.cppreference.com/cpp/language/alignas>

## What you learn

`alignas` is the dual of the `alignof` operator from step 06.
- `alignof(T)` *asks* what `T`'s alignment is.
- `alignas(N)` *requests* alignment of at least `N`.

It's a specifier that goes on a type declaration, a variable, or a
struct member, and tells the compiler to align that thing to at least
the given value.

```cpp
struct alignas(64) cache_line { char data[64]; };   /* whole type is 64-aligned */
alignas(64) char buffer[64];                        /* one variable is 64-aligned */
struct s { alignas(64) char data[64]; };            /* just this member is 64-aligned */
```

The argument to `alignas` must be:
- a constant expression
- a positive power of 2
- not smaller than the type's natural alignment — you can request
  more, never less

## Why does this exist?

The classic use case is **cache lines** and **false sharing**. On
x86_64 an L1 cache line is 64 bytes wide. If two threads each write
to two different variables that happen to share a cache line, the
cache-coherency protocol bounces the line between cores on every
write, even though the data they touch is logically disjoint. Forcing
the variables into separate 64-byte-aligned slots eliminates the
ping-pong. The canonical idiom is `struct alignas(64) ...`.

Also useful for SIMD types (16- or 32-byte alignment requirements),
and any time you need stricter alignment than the natural one.

## Task

In `solution.hpp`, fix the declaration of `task07::cache_line` so it
satisfies:

| Property                       | Required value |
|--------------------------------|----------------|
| `alignof(cache_line)`          | `64`           |
| `sizeof(cache_line)`           | `64`           |
| has a single member `data`     | of type `char[64]` |

`get_instance()` is already implemented for you — it returns a
pointer to a function-local `static` instance. The runtime tests
check that the returned address (and adjacent array elements) land
on 64-byte boundaries.

## Run

```sh
./verify.sh
```

## Hints

- The standard syntax: `struct alignas(64) cache_line { char data[64]; };`.
- Static-storage-duration variables get their type's natural alignment
  automatically, so `static cache_line instance{};` is aligned without
  further annotation — that's why `get_instance()` is one line.
- `static_assert(alignof(T) == 64, ...)` proves the *type* is aligned.
  The runtime check on `reinterpret_cast<uintptr_t>(p) % 64 == 0`
  proves an actual *instance* lands on a 64-byte boundary.

## Things you might try (optional)

- `alignas(7) struct x { ... };` — the compiler will complain.
  Alignments must be powers of 2.
- `alignas(1) int x;` — silently ignored. You can't request *less*
  alignment than the natural one.
- Declare `task07::cache_line arr[3]` and verify `&arr[1] - &arr[0]`
  is exactly `64`. Tight packing only works when `sizeof` is a
  multiple of `alignof`.
