# 90 — `std::iota`

> Spec: <https://en.cppreference.com/cpp/algorithm/iota>

## What you learn

`std::iota` fills a range with **monotonically increasing
values**, starting from a given seed:

```cpp
std::vector<int> v(10);
std::iota(v.begin(), v.end(), 0);
/* v == {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} */
```

The implementation is exactly:

```cpp
for (auto it = first; it != last; ++it) *it = value++;
```

What it gives you over a hand-rolled loop:

- One line. Intent is obvious from the name.
- Works on any forward range with a settable `operator*` --
  vectors, arrays, lists, raw arrays, custom containers.
- The `value` argument is **any** pre-incrementable type, so you
  can iota sequences of `unsigned long`, `char`, even your own
  index type with `operator++` defined.

### Why it lives in `<numeric>`

`std::iota` is not in `<algorithm>` -- it's in `<numeric>`,
alongside `accumulate`, `inner_product`, and `partial_sum`. The
historical reason: it's modelled on the APL/Iverson "iota"
primitive (`ι N` returns `1..N`), which is numerical in origin.
The standard library kept that lineage in its header
organisation.

Include it as:

```cpp
#include <numeric>
```

### Variants

There are no overloads on `std::iota`. If you want a *strided*
sequence (`0, 2, 4, 6, ...`), use `std::generate` with a small
stateful lambda:

```cpp
int n = 0;
std::generate(v.begin(), v.end(), [&n]() { int r = n; n += 2; return r; });
```

Or for floating-point sequences, use a similar generate lambda
with a `double` step. `std::iota` itself only does `value++`.

### Use cases beyond "fill 0..N"

- **Index permutations.** Initialise `idx = {0..n-1}`, then
  `std::sort(idx.begin(), idx.end(), [&](int a, int b) { return
  data[a] < data[b]; })` to sort indices by a comparison on the
  underlying data. Common pattern for "argsort".

- **Test data setup.** A vector of unique consecutive integers
  is a common test fixture.

- **Range adaptors.** Combined with `std::shuffle` (step 84),
  `std::iota` -> `std::shuffle` gives you a random permutation
  of 0..N-1 in two lines.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task90
{
    /* Return a vector<int> of size n filled with 0, 1, ..., n-1. */
    std::vector<int> range_n(std::size_t n);

    /* Return a vector<int> of size n filled with start, start+1, ...,
     * start+n-1. */
    std::vector<int> range_from(std::size_t n, int start);
}
```

Both are one-line `std::iota` calls into a pre-sized vector.

## Run

```sh
./verify.sh
```

## Hints

- The standard idiom:
  ```cpp
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), start);
  return v;
  ```

- For `range_n`, the start is `0`. Make sure you cast the literal
  to `int` if mixing with `std::size_t` -- the function takes
  `int`-typed seeds, and the lambda inside `iota` only cares about
  what `*it = value++` does.

- Include `<numeric>` (NOT `<algorithm>`). Compilers usually
  transitively-include enough headers that this works either
  way, but be explicit.

- `std::iota`'s `value` is passed by value and incremented inside
  the algorithm. Your caller's `start` variable is unchanged.

- The C++20 ranges library adds `std::views::iota(0, n)` which
  is a lazy view -- no allocation. For C++11, `std::iota` is
  the eager form that needs a pre-sized container.

- The name was kept (despite "iota" being obscure) because
  changing a standard library name is rarer than re-using a
  Greek letter from a 1960s notation. Read it as "fill with a
  sequence".
