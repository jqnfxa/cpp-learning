# 22 — Two-range overloads (`std::equal`, `std::mismatch`, `std::is_permutation`)

> Spec: <https://en.cppreference.com/cpp/algorithm/equal>

## What you learn

C++14 added "second-range-end" overloads to three classic
algorithms that previously took a one-and-a-half-range form:

| Algorithm           | C++11 form              | C++14 added            |
|---------------------|-------------------------|------------------------|
| `std::equal`        | `(b1, e1, b2)`          | `(b1, e1, b2, e2)`     |
| `std::mismatch`     | `(b1, e1, b2)`          | `(b1, e1, b2, e2)`     |
| `std::is_permutation`| `(b1, e1, b2)`         | `(b1, e1, b2, e2)`     |

The C++11 three-iterator form ASSUMES the second range is at
least as long as the first. If it isn't, the algorithm reads
past the end -- **undefined behaviour**.

The C++14 form takes both endpoints and bails out as soon as
either range runs out. Safer, more honest, and the natural
form when both ranges are known up front.

### The hazard the C++14 form fixes

Classic bug with the C++11 form:

```cpp
std::vector<int> a{1, 2, 3, 4, 5};
std::vector<int> b{1, 2, 3};

std::equal(a.begin(), a.end(), b.begin());     /* UB: reads past b's end */
```

`std::equal` walks `a` from begin to end, dereferencing `b`'s
iterator at each step. After three iterations, `b`'s iterator
points to `b.end()` -- dereferencing it is UB.

In debug mode, asan / valgrind / debug iterators catch this.
In release mode, you get garbage memory reads and possibly a
"true" return value (if the past-end bytes happen to match).

The C++14 form:

```cpp
std::equal(a.begin(), a.end(), b.begin(), b.end());   /* returns false: different lengths */
```

The size check happens first; same-length-required is the
implicit contract. If sizes differ, immediate `false`.

### Length-first short-circuit

For random-access iterators, the C++14 implementations
short-circuit on length:

```cpp
if (distance(b1, e1) != distance(b2, e2)) return false;
return equal_loop(b1, e1, b2);   /* lengths match -- safe */
```

For input-only / forward iterators, it walks both ranges in
parallel, returning as soon as either ends.

### `std::equal(b1, e1, b2, e2)` semantics

```cpp
bool equal(It1 b1, It1 e1, It2 b2, It2 e2);
```

Returns `true` iff:
- The two ranges have the same length, AND
- Every pair of corresponding elements is equal.

For different lengths: false (without reading past the
shorter range). For same length: behaviour matches the
C++11 form.

### `std::mismatch(b1, e1, b2, e2)` semantics

```cpp
std::pair<It1, It2> mismatch(It1 b1, It1 e1, It2 b2, It2 e2);
```

Returns the FIRST PAIR of positions where the ranges differ
(or where one range ends).

- If both ranges are exhausted with no difference: returns
  `{e1, e2}`.
- If the ranges differ at position `i`: returns
  `{b1+i, b2+i}`.
- If one range ends before any difference: returns the
  shorter range's end and the corresponding position in the
  longer range.

The "both match completely" check is `result == {e1, e2}` --
both iterators must hit their END, not just the shorter
one.

### `std::is_permutation(b1, e1, b2, e2)` semantics

```cpp
bool is_permutation(It1 b1, It1 e1, It2 b2, It2 e2);
```

Returns `true` iff [b1, e1) is a permutation of [b2, e2). The
length check is the first thing it does. If lengths differ,
immediate false.

For same length, the algorithm proceeds with the
O(n^2)-worst-case comparison from the C++11 form.

### When to use the three-arg form

The C++11 three-arg forms still exist; they aren't removed
in C++14. You can use them when:

- You KNOW the second range is at least as long as the
  first.
- You're operating on prefixes -- you only want to compare up
  to `e1`.

In modern code, prefer the four-arg form unless you're
explicit about "second range can be longer."

### Related: `std::lexicographical_compare` (C++20 spaceship)

C++20 added a four-arg `std::lexicographical_compare_three_way`
that returns `std::strong_ordering` instead of bool. For
C++14, the three-arg `lexicographical_compare` and the
new four-arg `equal`/`mismatch`/`is_permutation` are the
state of the art.

### Edge cases

- **Both empty**: `equal({}, {})` -> true. `mismatch({}, {})`
  -> `{e1, e2}`. `is_permutation({}, {})` -> true.
- **One empty**: `equal(non-empty, {})` -> false (different
  lengths).
- **Length 1**: works the same as longer ranges; size check
  succeeds for matching length.

## Task

In `solution.hpp`, fill in three template functions to use
the C++14 four-iterator forms:

```cpp
template <class It1, class It2>
bool safe_equal(It1 b1, It1 e1, It2 b2, It2 e2) {
    return std::equal(b1, e1, b2, e2);
}

template <class It1, class It2>
bool both_ranges_match_completely(It1 b1, It1 e1, It2 b2, It2 e2) {
    auto p = std::mismatch(b1, e1, b2, e2);
    return p.first == e1 && p.second == e2;
}

template <class It1, class It2>
bool safe_perm(It1 b1, It1 e1, It2 b2, It2 e2) {
    return std::is_permutation(b1, e1, b2, e2);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `<algorithm>` is the header. Already included.

- The TODO is in three places: each function. Just add `, e2`
  to the existing `std::equal` / `std::mismatch` /
  `std::is_permutation` call, and (for mismatch) check
  `p.second == e2` in addition to `p.first == e1`.

- The C++11 three-arg form takes `(b1, e1, b2)`. The C++14
  four-arg form takes `(b1, e1, b2, e2)`.

- `std::mismatch` returns a pair. For the "completely match"
  case, BOTH iterators must reach their END. The stub only
  checks `p.first == e1` -- which is true if `a` is a prefix
  of `b`, even when `b` is longer.

- For `equal_prefix_is_NOT_equal`: `[1,2,3]` is a prefix of
  `[1,2,3,4,5]`. The C++11 form returns true (just matched
  the first 3). The C++14 form returns false (lengths
  differ).

- For `equal_shorter_b_is_not_equal`: the reverse direction.
  With C++11, this is UB (reads past b's end). With C++14,
  returns false.

- Tests verify: same-size equal/different, prefix relations
  in both directions, empty ranges, mismatch
  "completely matches", permutation with same-set / different
  sizes / disjoint sets.

- These tests don't trigger UB on the stub for the
  asymmetric "longer b" case -- the past-end reads happen
  to fit within the std::vector's allocated capacity. But
  the behavior is wrong (returns true) and the test
  catches the result.

- The four-arg form is the safer default. Use it for new
  code; reserve the three-arg form for prefix-comparison
  cases where you intentionally want to compare just the
  first N.
