# 85 — `std::is_partitioned` / `std::partition_copy` / `std::partition_point`

> Spec: <https://en.cppreference.com/cpp/algorithm/is_partitioned>,
> <https://en.cppreference.com/cpp/algorithm/partition_copy>,
> <https://en.cppreference.com/cpp/algorithm/partition_point>

## What you learn

A range is **partitioned** by a predicate `pred` when:

```
[ pred(x) true ... ] [ pred(x) false ... ]
^                  ^ ^                   ^
first              | |                  last
                   middle (the "partition point")
```

That is, every element for which the predicate is true comes
*before* every element for which it's false. C++11 added three
algorithms that work with partitioned ranges:

```cpp
is_partitioned(first, last, pred);         /* -> bool */
partition_copy(first, last, true_out, false_out, pred);
partition_point(first, last, pred);        /* requires partitioned */
```

### `is_partitioned` — is this range partitioned by `pred`?

Walks once. Returns true iff every false-predicate element is
preceded only by true-predicate elements.

```cpp
std::vector<int> v{-3, -1, 0, 5, 8};
std::is_partitioned(v.begin(), v.end(), [](int x){ return x < 0; });
/* true: all negatives first, then all non-negatives */
```

Edge cases:
- Empty range: **true** (vacuously).
- All true: true (the "false" part is empty).
- All false: true (the "true" part is empty).

### `partition_copy` — split into two outputs

Walks the source once. Each element goes to one of two output
iterators based on `pred(x)`. Order is preserved within each
group.

```cpp
std::vector<int> neg, nonneg;
std::partition_copy(v.begin(), v.end(),
                    std::back_inserter(neg),       /* pred(x) true here */
                    std::back_inserter(nonneg),    /* pred(x) false here */
                    [](int x){ return x < 0; });
```

Useful any time you want to bucket a range into two streams
without mutating the source.

### `partition_point` — find the boundary in a partitioned range

Given a range already partitioned by `pred`, returns an iterator
to the **first element for which `pred` is false** — i.e. the
boundary between the two halves. Uses binary search internally
(O(log n) comparisons), so it's much faster than walking.

```cpp
auto it = std::partition_point(v.begin(), v.end(),
                                [](int x){ return x < 0; });
auto index = it - v.begin();
/* index == number of negatives */
```

**Precondition**: the range *must* already be partitioned by
`pred`. Calling `partition_point` on an unpartitioned range is
UB. Use `is_partitioned` first if you're not sure.

### The pairing

These three algorithms come together: you build a partitioned
range, you verify it with `is_partitioned`, you split it with
`partition_copy`, and you find the boundary in O(log n) with
`partition_point`. Each does one job that the other two can't
do directly.

## Task

In `solution.hpp`, implement three functions over
`std::vector<int>`, all using the predicate "x is negative":

```cpp
namespace task85
{
    /* True iff every negative comes before every non-negative in v. */
    bool is_partitioned_by_sign(const std::vector<int> &v);

    /* Bucket v into (negatives, non_negatives), preserving order
     * within each bucket. */
    std::pair<std::vector<int>, std::vector<int>>
    split_by_sign(const std::vector<int> &v);

    /* PRECONDITION: v must be partitioned by sign (negatives first).
     * Returns the index of the first non-negative element, or v.size()
     * if there are no non-negatives. */
    std::size_t partition_point_by_sign(const std::vector<int> &v);
}
```

The predicate is `[](int x) { return x < 0; }` in all three.

## Run

```sh
./verify.sh
```

## Hints

- `is_partitioned_by_sign` is one line:
  `return std::is_partitioned(v.begin(), v.end(), [](int x){ return x < 0; });`.

- `split_by_sign` uses `std::partition_copy` with two
  `back_inserter`s. The first output goes to negatives, the
  second to non-negatives. Don't swap them -- the predicate
  decides which output a given element lands in.

- `partition_point_by_sign` returns an iterator; convert to an
  index via `it - v.begin()`. For "all negatives" the iterator
  is `v.end()` and the index is `v.size()` -- exactly the
  "no non-negatives" answer.

- Don't call `partition_point` on an unpartitioned range. The
  test does **not** do that: its inputs are always partitioned.
  Adding an `assert(is_partitioned(...))` inside is optional but
  reasonable.

- `partition_point` is O(log n) when applied to a
  random-access-iterator range; O(n) for forward iterators.
  Vector is random-access, so this is the fast path.

- The classic real use: keep a vector pre-partitioned by some
  cheap key (e.g. "is this entry stale?"). Boundary lookups
  stay O(log n), partition merges are O(n) amortised, no
  separate index structure needed.
