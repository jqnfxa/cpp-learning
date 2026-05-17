# 79 — `std::move_iterator`

> Spec: <https://en.cppreference.com/cpp/iterator/move_iterator>

## What you learn

A `std::move_iterator<It>` is an iterator adapter: given any
input iterator, `*move_iterator(it)` yields a *T &&* instead of
the underlying `*it`'s reference type. Any algorithm or
container constructor that consumes iterators will then
*move-construct* its elements instead of *copy-constructing*
them.

```cpp
std::vector<std::string> src = {"a long string", "another long one"};

std::vector<std::string> dst(std::make_move_iterator(src.begin()),
                             std::make_move_iterator(src.end()));
/* Every element of src is now moved-from. dst owns the heap
   buffers that src used to own. */
```

The two key facts:

1. **It's an iterator, not a one-shot.** It wraps an existing
   iterator and changes `operator*`. Everything else (++, ==,
   etc.) forwards to the wrapped iterator.

2. **The source elements are mutated.** Move semantics happen
   when the consumer constructs (or assigns) from a `T &&`.
   Wrapping the source iterator in a `move_iterator` is what
   *makes* `*it` an rvalue; the consumer's rvalue-aware overload
   then takes ownership.

### Where it's useful

The most common use is range constructors of containers:

```cpp
/* Bulk-move source range into a new container. */
std::vector<std::string> dst(std::make_move_iterator(src.begin()),
                             std::make_move_iterator(src.end()));
```

```cpp
/* Bulk-move into an existing container. */
dst.insert(dst.end(),
           std::make_move_iterator(src.begin()),
           std::make_move_iterator(src.end()));
```

```cpp
/* Move-aware algorithm: std::copy on move_iterators is essentially
   std::move(first, last, out). */
std::copy(std::make_move_iterator(src.begin()),
          std::make_move_iterator(src.end()),
          std::back_inserter(dst));
```

The standard library provides `std::move(first, last, out)` (in
`<algorithm>`) as the convenience form of the third pattern;
`move_iterator` is the underlying mechanism.

### What it's not

`std::move_iterator` doesn't deep-clone the source range. It
doesn't erase or resize the source. It moves *element by
element*. After the bulk move, `src` still has the same size and
its elements are in a "valid but unspecified" (typically empty)
state. If you want them gone too, `src.clear()`.

### Why the helper exists

`make_move_iterator(it)` deduces the iterator type for you, the
same way `std::make_pair`, `std::make_shared`, etc. deduce their
template arguments. Spelling
`std::move_iterator<std::vector<std::string>::iterator>(src.begin())`
explicitly works but is unbearable. C++17 added Class Template
Argument Deduction (CTAD) which makes
`std::move_iterator(src.begin())` work too; for C++11 you stick
with `make_move_iterator`.

## Task

In `solution.hpp`, implement two factories:

```cpp
namespace task79
{
    /* Build dst by copying every element of src.  src is unchanged. */
    std::vector<std::string> via_copy_range(std::vector<std::string> &src);

    /* Build dst by moving every element of src.  After the call, every
     * element of src is in a moved-from state (empty for heap-allocated
     * strings, on libstdc++/libc++). */
    std::vector<std::string> via_move_range(std::vector<std::string> &src);
}
```

Both functions take `src` by **non-const reference** because the
move-from path needs to mutate the source — the lvalue overload
otherwise would bind.

## Run

```sh
./verify.sh
```

## Hints

- `via_copy_range` is a one-liner: pass `src.begin()` and
  `src.end()` to the vector's range constructor.

- `via_move_range` is the same shape but wraps each iterator in
  `std::make_move_iterator(...)`.

- For the tests to show heap-buffer transfer, every string in
  `src` is longer than the SSO buffer of any major STL (~32
  chars is enough). Captured `s.data()` pointers should appear
  in `dst` after the move.

- `std::make_move_iterator` lives in `<iterator>` (it's an
  iterator adapter, not in `<algorithm>`). `std::move(first,
  last, out)` -- the *algorithm*, not the cast -- lives in
  `<algorithm>`; both forms produce the same effect.

- `move_iterator` and `back_inserter` compose. The pattern
  ```cpp
  std::copy(std::make_move_iterator(src.begin()),
            std::make_move_iterator(src.end()),
            std::back_inserter(dst));
  ```
  is a fine alternative to a range constructor when `dst`
  already exists and you don't want to wipe it first.

- After the move, do *not* read the values of source elements as
  if they were the originals. Each is a valid string object
  (default-constructible, assignable, destructible) but its
  *content* is unspecified. The standard does not promise
  "empty".

- The trade-off between `move_iterator` and `emplace_back` in a
  loop: `move_iterator` is bulk and shorter; the loop-with-
  `emplace_back` form gives you per-element customisation. For
  "move N strings from src to dst", `move_iterator` wins on
  brevity.
