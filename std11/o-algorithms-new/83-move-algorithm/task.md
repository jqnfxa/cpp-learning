# 83 — `std::move` (algorithm) and `std::move_backward`

> Spec: <https://en.cppreference.com/cpp/algorithm/move>,
> <https://en.cppreference.com/cpp/algorithm/move_backward>

## What you learn

This step is about the **two algorithms** named like the cast,
not the cast itself. They live in `<algorithm>` (not `<utility>`,
which is where `std::move` the cast lives). Annoying that the
names collide; clearer once you know.

```cpp
template <class It1, class It2>
It2 move(It1 first, It1 last, It2 d_first);   /* forward */

template <class It1, class It2>
It2 move_backward(It1 first, It1 last, It2 d_last);   /* reverse */
```

Both do "element-by-element move", which is what you get if you
write `std::copy` and replace each `*d_first++ = *first++` with
`*d_first++ = std::move(*first++)`. So:

```cpp
std::move(first, last, d_first);
/* ⇔  while (first != last) *d_first++ = std::move(*first++); */
```

The result: every element in `[first, last)` is move-assigned
(or move-constructed via `back_inserter`) into the destination,
leaving the source elements in a valid-but-unspecified state.

### The forward form: `std::move(first, last, d_first)`

The canonical use is "move a range into a fresh vector":

```cpp
std::vector<std::string> dst;
dst.reserve(src.size());
std::move(src.begin(), src.end(),
          std::back_inserter(dst));
/* dst now owns the strings; src's elements are moved-from. */
```

This is the algorithmic counterpart of step 79's
`std::make_move_iterator` + range constructor. The two forms
do the same work, with different ergonomics.

### The backward form: `std::move_backward(first, last, d_last)`

`move_backward` reads from the *end* of the source range and
writes from the *end* of the destination, iterating *toward*
`first`. It's the right tool when source and destination
**overlap** and the destination starts *to the right* of the
source — moving forward would overwrite source elements before
they're read.

The classic example is "shift the contents right by one":

```cpp
std::vector<std::string> v{"A", "B", "C", "D"};
std::move_backward(v.begin(), v.end() - 1, v.end());
/* v == { moved-from-A, "A", "B", "C" }
   v[0] is in a valid-but-unspecified state.
   v[3] used to be "D"; it was overwritten by the moved-in "C".
   "D" is now gone. */
```

If you tried that with the *forward* `std::move`:

```cpp
std::move(v.begin(), v.end() - 1, v.begin() + 1);
/* hazardous: by the time we get to v[2] = std::move(v[1]),
   v[1] has already been moved-from when we did v[1] = std::move(v[0]).
   On strings the moved-from is typically empty, so v[2] becomes empty
   instead of the original v[1]. */
```

Move-backward + ending iterator avoids this exactly the way
`memmove` does for overlapping `memcpy`s.

### Comparison with the iterator-adapter form

Step 79's `std::make_move_iterator` adapts the input iterator;
the consumer (range constructor, etc.) sees rvalues. The
algorithms in this step are pre-packaged "consumer + adapter"
bundles. Use whichever reads cleanly in the call site.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task83
{
    /* Move every element of src into a fresh destination vector via
     * std::move (the algorithm). After the call src's elements are
     * moved-from (empty for heap-allocated strings). */
    std::vector<std::string> move_into_new(std::vector<std::string> &src);

    /* Shift v's elements one position to the right, using
     * std::move_backward. The last element is overwritten (lost);
     * v[0] is left in a moved-from state. Vectors of size 0 or 1 are
     * unchanged. */
    void shift_right_by_one(std::vector<std::string> &v);
}
```

## Run

```sh
./verify.sh
```

## Hints

- For `move_into_new`, two lines: reserve dst.capacity, then
  `std::move(src.begin(), src.end(), std::back_inserter(dst));`.
  Yes, `back_inserter` works for `std::move` — its `operator*=`
  proxy calls `push_back`, which has an rvalue overload.

- For `shift_right_by_one`, handle the n < 2 case first (no-op).
  Then call `std::move_backward(v.begin(), v.end() - 1, v.end());`.
  This reads `v[n-2], v[n-3], ..., v[0]` and writes
  `v[n-1], v[n-2], ..., v[1]`.

- The reason you need `move_backward` (not `move`) for the
  right-shift: if you used forward `std::move(v.begin(),
  v.end() - 1, v.begin() + 1)`, the second iteration would read
  `v[1]`, which the first iteration already moved from. The
  source values are gone before they're read. `move_backward`
  iterates the opposite direction, dodging that.

- For long strings (busting SSO), the move transfers heap
  buffers. The tests capture `.data()` pointers before the move
  and assert that they appear at the expected destinations,
  proving a real move (not a copy).

- These algorithms live in `<algorithm>`. The cast `std::move(x)`
  lives in `<utility>`. Both names are spelled identically;
  context (argument shape) disambiguates.

- For non-overlapping ranges, forward `std::move` is fine. For
  overlapping ranges where the destination is to the right of
  the source, you need `move_backward`. For the opposite
  overlap (destination to the left), forward `std::move` works.
