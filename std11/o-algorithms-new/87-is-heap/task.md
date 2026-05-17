# 87 — `std::is_heap` / `std::is_heap_until`

> Spec: <https://en.cppreference.com/cpp/algorithm/is_heap>,
> <https://en.cppreference.com/cpp/algorithm/is_heap_until>

## What you learn

A **heap** in `<algorithm>` is the same idea as the standard
array-backed binary heap: element `i` is the parent of elements
`2i+1` and `2i+2`. The default comparator is `<`, which produces
a **max-heap**: every parent ≥ its children. (Pass
`std::greater<T>()` for a min-heap.)

```
       9
      / \
     5   4
    / \ /
   2  3 1
```

Stored as `{9, 5, 4, 2, 3, 1}`. Heap property: 9 ≥ 5, 9 ≥ 4, 5 ≥
2, 5 ≥ 3, 4 ≥ 1. Yes.

C++11 added two query algorithms:

```cpp
bool is_heap     (first, last);    /* "is the range a max-heap?" */
It   is_heap_until(first, last);   /* iterator to first violator,
                                      or last if it's a heap */
```

Same pairing as `is_sorted` / `is_sorted_until` from step 86.

### What "violator" means

`is_heap_until` returns the **first element that breaks the
heap property** — the iterator points at the *child* whose
parent is too small. If the whole range is a heap, it returns
`last`.

```cpp
std::vector<int> v{9, 5, 4, 8, 3, 1};
auto it = std::is_heap_until(v.begin(), v.end());
/* Heap check: parent v[1]=5; children v[3]=8, v[4]=3.
   8 > 5 -- violation. it points at v[3] = 8. */
```

### Construction by `std::make_heap`

To build a guaranteed-valid heap for testing:

```cpp
std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
std::make_heap(v.begin(), v.end());
/* v is now a max-heap. */
```

`make_heap` runs in O(n) (Floyd's algorithm), not O(n log n) —
a nice constant-time-per-element rearrangement. The standard
PriorityQueue (`std::priority_queue`) is built on this family
of algorithms.

### Empty and single-element ranges

Both vacuously hold the heap property: `is_heap == true`,
`is_heap_until == last`. Single element trivially has no
parent-child pairs to violate.

### Why both algorithms exist

`is_heap` is the cheap "is it OK?" check; `is_heap_until` is
the diagnostic that tells you *where* to start fixing things.
The library pattern is:

```cpp
auto good = std::is_heap_until(first, last);
if (good != last)
{
    /* re-heap from somewhere -- either fix in place with
       push_heap / sift down, or just `make_heap` the whole thing. */
}
```

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task87
{
    /* True iff v satisfies the max-heap property under <. */
    bool is_max_heap(const std::vector<int> &v);

    /* Return the index of the first element that violates the max-heap
     * property, or v.size() if v is a valid heap. */
    std::size_t first_heap_violator(const std::vector<int> &v);
}
```

`is_max_heap` is one line. `first_heap_violator` converts the
iterator from `is_heap_until` into an index, with `v.size()` as
the "no violator" sentinel.

## Run

```sh
./verify.sh
```

## Hints

- `is_max_heap` is `return std::is_heap(v.begin(), v.end());`.

- `first_heap_violator` follows the same shape as step 86's
  `first_descent`:
  ```cpp
  auto it = std::is_heap_until(v.begin(), v.end());
  return static_cast<std::size_t>(it - v.begin());
  ```

- The default comparator is `<`, which checks the max-heap
  property. Don't try to use `<=` -- it isn't a strict weak
  order and the algorithm's guarantees go out the window.

- For a min-heap, you'd pass `std::greater<int>()` as the third
  argument to both algorithms. That's not part of this task --
  we stick to max-heap throughout.

- The tests use `std::make_heap` to construct guaranteed-valid
  heap inputs. If you find yourself manually writing a heap,
  remember the parent/child rule: for index `i`, parent is at
  `(i - 1) / 2`, children at `2i + 1` and `2i + 2`. Index 0 is
  the root.

- `make_heap` runs in O(n). `push_heap` and `pop_heap` are
  O(log n). `sort_heap` is O(n log n). Together these are the
  C++ building blocks for a binary heap; `priority_queue` is the
  RAII wrapper around them.

- The heap defined by `<algorithm>` is the same heap used by
  `std::priority_queue` and the same as the textbook
  array-backed binary heap. Knowing this set of algorithms is
  knowing how `priority_queue` works internally.
