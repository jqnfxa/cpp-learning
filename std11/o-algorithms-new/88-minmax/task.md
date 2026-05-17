# 88 — `std::minmax` and `std::minmax_element`

> Spec: <https://en.cppreference.com/cpp/algorithm/minmax>,
> <https://en.cppreference.com/cpp/algorithm/minmax_element>

## What you learn

Three overloads, all returning the min and the max together in
one `std::pair`:

```cpp
std::pair<T, T>   minmax(a, b);                  /* two values        */
std::pair<T, T>   minmax({a, b, c, ...});        /* initializer_list  */
std::pair<It, It> minmax_element(first, last);    /* range -> iterators */
```

### Why "both at once" matters

The single-pass guarantee. Compare:

```cpp
auto lo = std::min_element(v.begin(), v.end());   /* one pass */
auto hi = std::max_element(v.begin(), v.end());   /* second pass */
```

vs.

```cpp
auto pr = std::minmax_element(v.begin(), v.end()); /* one pass, ~3n/2 comps */
```

For large ranges or expensive comparators, the difference is
visible. The combined algorithm uses ~⌈3n/2⌉ - 2 comparisons
(comparing in pairs first, then against the current min/max).
The split form does ~2(n-1).

### Tie-breaking convention

When values are equal, the convention is asymmetric:

- `minmax(a, b)`: if `a` and `b` compare equal, `a` is "first"
  (the min) and `b` is "second" (the max). This means
  `minmax(a, a).first == a` and `.second == a`.
- `minmax_element(...)` returns *iterators*. On ties:
  - `min` is the iterator to the **first** element equal to the
    minimum.
  - `max` is the iterator to the **last** element equal to the
    maximum.

This asymmetry is for sorting stability — knowing which element
the iterator-pair came from matters when the elements are not
just numbers.

### The two-value form

```cpp
auto pr = std::minmax(7, 3);
/* pr.first == 3, pr.second == 7 */
```

Always returns by value. For larger types, prefer
`minmax_element` over building a temporary container.

### The initializer-list form

```cpp
auto pr = std::minmax({4, 1, 7, 3, 9, 2});
/* pr.first == 1, pr.second == 9 */
```

Useful for "give me the min/max of these specific four things"
without making a vector. C++11's `<algorithm>` overloaded
several functions (`min`, `max`, `minmax`) to accept
initializer_lists for this reason.

### Empty ranges

`minmax_element` on an empty range returns `{last, last}` — the
caller must check before dereferencing. The other two forms can
never be called with "no values"; the language and library both
require at least two for `minmax(a, b)` and at least one
element in the initializer_list (passing `{}` is ill-formed).

## Task

In `solution.hpp`, implement three functions:

```cpp
namespace task88
{
    /* Two-value form: returns (min(a,b), max(a,b)). */
    std::pair<int, int> minmax_pair(int a, int b);

    /* Initializer-list form: returns (min, max) over the list. */
    std::pair<int, int> minmax_list(std::initializer_list<int> xs);

    /* Range form, one pass: returns (min, max) of v.
     * Precondition: v is non-empty. */
    std::pair<int, int> minmax_range(const std::vector<int> &v);
}
```

Each is one short call into the matching `<algorithm>` entry.
For `minmax_range`, use `minmax_element` and dereference the
returned iterator pair into the value pair the function
declares.

## Run

```sh
./verify.sh
```

## Hints

- `minmax_pair`: `return std::minmax(a, b);` -- the algorithm
  returns `std::pair<const int &, const int &>` for the two-arg
  form, which converts to `std::pair<int, int>` on return.
  Actually safe to just use return as-is; the conversion is
  fine.

- `minmax_list`: `return std::minmax(xs);` -- accepts
  `initializer_list<T>`. Same return type as above (by value
  by the time the function returns).

- `minmax_range` is two lines:
  ```cpp
  auto pr = std::minmax_element(v.begin(), v.end());
  return {*pr.first, *pr.second};
  ```

- The tests don't probe the iterator-tie convention -- they use
  inputs with unique extrema so the iterator-vs-value
  distinction doesn't matter.

- `std::pair<int, int>` constructs from a brace-init list `{a, b}`
  in C++11. That's the convenient return spelling.

- C++17 introduced `std::clamp(v, lo, hi)`, which is "minmax in
  reverse": ensure `v` is between `lo` and `hi`. Not part of
  this task but worth knowing exists.

- The combined min/max in one pass shows up regularly in
  performance-sensitive code (image processing, signal
  processing). The default-implementation savings vs separate
  min and max calls are real -- about 25% fewer comparisons.
