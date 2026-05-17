# 82 — `std::copy_if` and `std::copy_n`

> Spec: <https://en.cppreference.com/cpp/algorithm/copy>

## What you learn

C++11 added two new copy variants to `<algorithm>`:

```cpp
copy_if(first, last, out, pred);       /* like copy, but only when pred(*it) */
copy_n  (first, n,    out);             /* copy exactly n elements */
```

Both write to an output iterator. The usual choice for "append
into a vector" is `std::back_inserter(v)`, which wraps `v` so
each assignment becomes a `push_back`.

### `copy_if`

```cpp
std::vector<int> evens;
std::copy_if(v.begin(), v.end(),
             std::back_inserter(evens),
             [](int x) { return x % 2 == 0; });
```

It's `for (auto x : v) if (pred(x)) out.push_back(x);` in
algorithm form. The advantage is the same one as `all_of` etc.:
intent in the name, no manual indexing, fewer chances to write
the loop bound wrong.

### `copy_n`

```cpp
std::vector<int> head;
std::copy_n(v.begin(), 5, std::back_inserter(head));
```

It copies exactly `n` elements starting at `first`. The caller
must guarantee `n <= distance(first, last)` — `copy_n` does
**not** check the source range's end. Walking past the end is
undefined behaviour.

The usual safe spelling clamps `n`:

```cpp
std::size_t n = std::min(k, v.size());
std::copy_n(v.begin(), n, std::back_inserter(head));
```

### Why both exist

Before C++11 you wrote:

```cpp
std::vector<int> evens;
for (auto x : v) if (x % 2 == 0) evens.push_back(x);
```

or, with the old `remove_copy_if` (which copies elements that
*fail* the predicate — note the inversion), the algorithm-form
was awkward enough that most code stayed loop-based.
`copy_if` cleaned this up; `copy_n` filled in the "exactly n,
no end iterator needed" niche that streams and limited-size
copies want.

### Output-iterator basics

`std::back_inserter(c)` returns a `back_insert_iterator<C>` that
turns assignments into `c.push_back(...)`. There's also:

- `std::front_inserter(c)` -- for `deque` / `list` /
  `forward_list`.
- `std::inserter(c, pos)` -- general, inserts at iterator
  position.
- Or write into an existing range whose size you already know.

For this task, `back_inserter` is the right choice.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task82
{
    /* Return a new vector containing only the even elements of v,
     * in original order. */
    std::vector<int> evens(const std::vector<int> &v);

    /* Return a new vector containing the first min(k, v.size())
     * elements of v. */
    std::vector<int> first_n(const std::vector<int> &v, std::size_t k);
}
```

`evens` uses `std::copy_if` with a `[](int x) { return x % 2 == 0; }`
predicate. `first_n` uses `std::copy_n` with a clamped count.

## Run

```sh
./verify.sh
```

## Hints

- For `evens`, the typical shape is:
  ```cpp
  std::vector<int> out;
  std::copy_if(v.begin(), v.end(),
               std::back_inserter(out),
               [](int x) { return x % 2 == 0; });
  return out;
  ```

- For `first_n`, clamp `k` against `v.size()` before calling
  `copy_n`. Passing `k > v.size()` would read past the end --
  UB. The fix is one line:
  `std::size_t n = std::min(k, v.size());`.

- For negative numbers and the `% 2` test: `-2 % 2 == 0` in
  C++11 (and C99). The truncated-towards-zero rule means `-3 %
  2 == -1` and `-2 % 2 == 0`, so `x % 2 == 0` is the right
  evenness test for both signs.

- `out.reserve(v.size())` before `copy_if` is a fine
  optimisation -- at worst every element is kept, so reserving
  avoids reallocations. Not required.

- `std::back_inserter` lives in `<iterator>`. Most code that
  uses `back_inserter` also includes `<algorithm>` for the
  algorithm side; the two go together.

- `std::copy_if` made its way through several committee
  proposals before C++11 because there was disagreement about
  predicate semantics (positive vs negative). The C++11 form
  matches the obvious intuition: copy the elements for which
  the predicate is *true*.
