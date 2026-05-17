# 86 — `std::is_sorted` and `std::is_sorted_until`

> Spec: <https://en.cppreference.com/cpp/algorithm/is_sorted>,
> <https://en.cppreference.com/cpp/algorithm/is_sorted_until>

## What you learn

Both new in C++11, these two answer "is this range sorted?" in
the two natural ways:

```cpp
bool is_sorted(first, last);           /* yes/no */
It   is_sorted_until(first, last);     /* the iterator to the first
                                          element that breaks the order */
```

For the default comparator `operator<`, an "ascending-sorted"
range means `*it <= *(it+1)` for every adjacent pair (equivalently,
no `*it > *(it+1)` -- and the default uses `<`, so the predicate
"out of order" is `*next < *it`).

### `is_sorted_until` — the more general one

`is_sorted_until(first, last)` walks the range until it finds a
pair `(*it, *(it+1))` where the second element is strictly less
than the first (under `<`), then returns *the iterator to that
second element* — i.e. the **first element that violates the
sort**. If the whole range is sorted, it returns `last`.

```cpp
std::vector<int> v{1, 2, 3, 1, 4};
auto it = std::is_sorted_until(v.begin(), v.end());
/* it points to v[3] == 1, the descent point. */
```

That position `it - begin` is the **length of the longest
sorted prefix**, and equivalently the **index of the first
descent**.

`is_sorted(first, last)` is just `is_sorted_until(...) == last`.

### Comparators and ordering convention

Both algorithms take an optional comparator as a third argument.
The convention is the same as `std::sort`: pass a strict weak
ordering, default `<`. To check descending-sortedness:

```cpp
std::is_sorted(v.begin(), v.end(), std::greater<int>());
```

The "first descent" question is exactly "first position where
`comp(*next, *prev)` is true" — which is what `is_sorted_until`
finds.

### Edge cases worth knowing

- **Empty range**: both return `is_sorted == true`,
  `is_sorted_until == last`. Vacuous truth again.
- **Single-element range**: same as empty for these purposes —
  no adjacent pair exists.
- **Repeats**: under `<`, `{2, 2, 2}` is sorted (no strict
  violation). Under `<=`, repeats would *break* the strict
  weak order requirement — never use `<=` as a `sort` /
  `is_sorted` comparator.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task86
{
    /* True iff v is sorted ascending under <. */
    bool is_sorted_asc(const std::vector<int> &v);

    /* Return the index of the first element that breaks the ascending
     * order (i.e. the first v[i] with v[i] < v[i-1]). Returns v.size()
     * if v is fully sorted. */
    std::size_t first_descent(const std::vector<int> &v);
}
```

`is_sorted_asc` is one line. `first_descent` uses
`is_sorted_until` and converts the iterator to an index.

## Run

```sh
./verify.sh
```

## Hints

- `is_sorted_asc` is `return std::is_sorted(v.begin(), v.end());`.

- `first_descent` is two lines:
  ```cpp
  auto it = std::is_sorted_until(v.begin(), v.end());
  return static_cast<std::size_t>(it - v.begin());
  ```

- If the range is fully sorted, `is_sorted_until` returns `end()`
  and the index comes out as `v.size()` -- exactly the
  not-found sentinel you want.

- The conventional sentinel is `v.size()` (not `npos` or `-1`)
  because `std::size_t` is unsigned and the caller can ask "is
  it less than `v.size()`?" without worrying about
  underflow-from-`-1`.

- For descending sort, pass `std::greater<int>()` as the third
  argument. We stick to the default ascending here.

- `is_sorted_until` is the older "find the first out-of-order
  pair" primitive. `is_sorted` is the convenience boolean that
  layers on top. Knowing which to use depends on whether you
  want to *act on* the boundary or just *check* the property.

- Implementation detail: both run in O(n) with one pass through
  the range. They short-circuit at the first violation, so
  worst case is fully-sorted (must check every pair).
