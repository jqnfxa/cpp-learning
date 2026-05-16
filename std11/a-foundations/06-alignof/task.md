# 06 — `alignof`

> Spec: <https://en.cppreference.com/cpp/language/alignof>

## What you learn

**`alignof(T)`** is a compile-time unary operator that returns the
alignment requirement of `T` as a `std::size_t`. Like `sizeof`, it's
a constant expression — usable inside `static_assert`, as a template
argument, anywhere you can use a constant.

### The rules the standard pins down

1. **Alignment is a positive power of 2.** It is at least `1`.
2. **`alignof(char) == 1`.** This is the *only* fundamental-type
   alignment the standard fixes by name.
3. **`sizeof(T)` is a multiple of `alignof(T)`** for every complete
   object type `T`. (So an aligned array `T arr[N]` has every element
   landing on a proper boundary without extra padding between elements.)
4. **A struct/class has the alignment of its most strictly aligned
   member.** Implementations may add more for ABI reasons, but never less.

### What's *not* guaranteed (and varies by platform)

- The exact `alignof(int)`, `alignof(double)`, etc. On 64-bit Linux
  with x86_64 you'll typically see:

  | Type          | `sizeof` | `alignof` |
  |---------------|----------|-----------|
  | `char`        | 1        | 1         |
  | `int`         | 4        | 4         |
  | `double`      | 8        | 8         |
  | `long double` | 16       | 16        |

  On Apple silicon or 32-bit platforms numbers will differ.

### Why the struct in this task is interesting

```cpp
struct mixed {
    char a;     /* offset 0 */
    int  b;     /* must be int-aligned (typically offset 4) */
    char c;     /* offset 8 */
};              /* sizeof is rounded up to a multiple of alignof(mixed) */
```

A naive read suggests `1 + 4 + 1 = 6` bytes. The compiler is forced to:
- Pad after `a` so that `b` sits on a multiple of `alignof(int)`.
- Pad at the tail so the total size is a multiple of `alignof(mixed)`,
  which is `alignof(int)`. Otherwise `mixed arr[2]` would put `arr[1].b`
  off-boundary.

On a typical x86_64 platform `sizeof(mixed)` is `12`, not `6`.

## Task

The struct `task06::mixed` is already declared in `solution.hpp`. You
implement six wrapper functions that surface `alignof` and `sizeof`
as compile-time constants:

```cpp
constexpr std::size_t alignment_of_char();
constexpr std::size_t alignment_of_int();
constexpr std::size_t alignment_of_double();
constexpr std::size_t alignment_of_long_double();
constexpr std::size_t alignment_of_mixed();
constexpr std::size_t size_of_mixed();
```

Each is a one-liner. The point is that `alignof(T)` is a compile-time
operator — `tests.cpp` proves it by using your functions inside
`static_assert`.

### Tests

The compile-time block in `tests.cpp` asserts the *portable*
properties — `alignof(char) == 1`, `sizeof % alignof == 0`,
`alignof(mixed) == alignof(int)`, `sizeof(mixed) > 1 + 4 + 1`
(i.e. the compiler had to insert padding).

There is also a runtime "print" test that dumps the actual values for
your platform — useful to *see* what `alignof` gives you locally, as
the plan hint asked for.

## Run

```sh
./verify.sh
```

## Hints

- Each function body is a single `return alignof(T);` or
  `return sizeof(T);`. That's the whole task.
- `alignof` does *not* take an expression — only a type. `alignof x`
  where `x` is a variable is invalid; you write `alignof(decltype(x))`
  or `alignof(int)`.
- C++11 `constexpr` rules apply (single return, no locals) — but each
  function is already so simple this isn't a constraint.
