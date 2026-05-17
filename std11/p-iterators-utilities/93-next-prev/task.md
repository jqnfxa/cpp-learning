# 93 — `std::next` and `std::prev`

> Spec: <https://en.cppreference.com/cpp/iterator/next>,
> <https://en.cppreference.com/cpp/iterator/prev>

## What you learn

C++11 added `std::next(it, n)` and `std::prev(it, n)` to
`<iterator>`. They return *a copy* of the iterator advanced /
retreated by `n` steps, without mutating the original:

```cpp
auto it = v.begin();
auto third = std::next(it, 2);   /* `it` unchanged; `third` points 2 past it */
```

Compared to the pre-C++11 alternatives:

| Pre-C++11                          | C++11                       | Mutates `it`? |
|------------------------------------|-----------------------------|---------------|
| `it + 2` (random-access only)      | same                        | no            |
| `std::advance(it, 2);`              | same                        | **yes**       |
| (no concise non-mutating spelling)  | `std::next(it, 2)`           | no            |

`std::advance` mutates in place; that's surprising in
expressions. `std::next` and `std::prev` give you the
non-mutating return.

### Default count

Both default `n = 1`:

```cpp
auto second = std::next(it);  /* same as std::next(it, 1) */
auto last_minus_one = std::prev(v.end());
```

### Complexity

- Random-access iterators (vector, array, deque, raw pointers):
  O(1). Implemented as `it + n`.
- Bidirectional iterators (list, set, map): O(|n|). The
  algorithm walks step-by-step.
- Forward iterators (forward_list, istream_iterator): `next`
  is O(n) for positive `n`. `prev` is **not callable** on
  forward iterators (no `--`), so the compile error happens at
  use, which is what you want.

`std::next` is a *pure function* in the algorithmic sense:
its only effect is the returned iterator. Use it freely inside
larger expressions.

### Why this matters

Three uses come up a lot:

1. **Skip the first element** when iterating:
   ```cpp
   for (auto it = std::next(v.begin()); it != v.end(); ++it)
   ```

2. **The last element**:
   ```cpp
   auto last = std::prev(v.end());
   /* equivalent to v.back() iterator-form */
   ```

3. **Sub-ranges** for an algorithm:
   ```cpp
   std::sort(std::next(v.begin(), 2), std::prev(v.end(), 2));
   /* sort the middle, ignoring 2 on each side */
   ```

C++20 ranges has `views::drop(2)` and `views::drop_last(2)` for
the same thing without explicit iterator arithmetic.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task93
{
    /* Return v[k] using std::next on v.begin(). */
    int nth_element(const std::vector<int> &v, std::size_t k);

    /* Return v[v.size() - k] using std::prev on v.end().
     * I.e. nth_from_end(v, 1) is the last element, (v, 2) is the
     * second-to-last, etc. */
    int nth_from_end(const std::vector<int> &v, std::size_t k);
}
```

Both must use `std::next` / `std::prev` rather than `v[k]` or
manual arithmetic. The point is the standard-library spelling.

## Run

```sh
./verify.sh
```

## Hints

- `nth_element` body:
  ```cpp
  return *std::next(v.begin(), static_cast<std::ptrdiff_t>(k));
  ```
  The cast keeps the `unsigned -> signed` conversion explicit;
  `std::next`'s `n` parameter is signed (`difference_type`,
  which is signed for vector iterators).

- `nth_from_end` body:
  ```cpp
  return *std::prev(v.end(), static_cast<std::ptrdiff_t>(k));
  ```
  Same shape. `nth_from_end(v, 1)` calls `std::prev(end, 1)`,
  which gives the iterator to the last element.

- Preconditions: `k` must be in range. `nth_element` requires
  `k < v.size()`. `nth_from_end` requires `1 <= k <= v.size()`.
  The tests stay within bounds; out-of-range is UB and not
  part of the spec.

- Don't hand-roll a loop. The task is "advance by k without
  writing a hand loop" -- the answer is the function call.

- `std::advance(it, n)` is the *mutating* sibling. It returns
  `void` and changes `it`. Use it when you want to move an
  iterator you already named; use `std::next`/`std::prev` when
  you want a *new* iterator without touching the source.

- Range-based for is the modern alternative for "iterate
  everything". `std::next` shines for "iterate everything but
  the first" or "iterate a contiguous sub-range" without
  copying indices around.
