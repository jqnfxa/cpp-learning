# 123 — distributions: `uniform_int`, `uniform_real`

> Spec: <https://en.cppreference.com/cpp/numeric/random>

## What you learn

An **engine** (step 122) produces raw integer noise in its
native output range. A **distribution** maps that noise to a
useful shape: ints in `[1, 6]`, doubles in `[0, 1)`, normal
draws around a mean, etc.

The two simplest distributions are `uniform_int_distribution`
and `uniform_real_distribution`:

```cpp
std::mt19937 rng(42);

std::uniform_int_distribution<int> dice(1, 6);
int roll = dice(rng);                     /* int in [1, 6] inclusive */

std::uniform_real_distribution<double> unit(0.0, 1.0);
double r = unit(rng);                     /* double in [0.0, 1.0) */
```

The distribution is a **callable object**: you construct it
once with its parameters, then call it with an engine. The
distribution does the rejection sampling and bit-mixing to
turn engine output into samples in the requested range.

### `uniform_int_distribution<T>(a, b)`

- Range is **closed**: `[a, b]` (both endpoints inclusive).
- Equal probability for every value in the range.
- Default template parameter is `int`; use `std::uint64_t` etc.
  for wider ranges.

```cpp
std::uniform_int_distribution<int> d(1, 6);
/* 1, 2, 3, 4, 5, 6 each with probability 1/6 */
```

### `uniform_real_distribution<T>(a, b)`

- Range is **half-open**: `[a, b)` (b excluded by default).
- Equal probability density across the range.
- Default template parameter is `double`.

```cpp
std::uniform_real_distribution<double> d(0.0, 1.0);
/* values in [0.0, 1.0) */
```

The convention "low inclusive, high exclusive" for reals
matches what most stdlib `random` interfaces do (Python,
NumPy, etc.) — it's what makes "scale by N" produce values in
`[0, N)`.

### Why bother with distributions?

You *could* roll your own: `(engine() % 6) + 1` for dice.
That has two problems:

1. **Bias**. The engine's output range is rarely an exact
   multiple of your target range, so modulo introduces a tiny
   bias toward lower values. The standard distribution
   handles this via rejection sampling.

2. **Reproducibility across types**. `uniform_int_distribution`
   on the same seed produces the same sequence regardless of
   which engine you used (under one STL implementation). The
   modulo trick depends on the engine's output range.

Distributions cost a few extra instructions per call and
remove these traps. Always use them.

### Histogram of 1000 dice rolls

The textbook demo: roll a 6-sided die 1000 times, count the
buckets. With a fair die, each bucket should land near
1000/6 ≈ 167. Sampling variance means the actual buckets vary
by ±20 or so.

```cpp
std::mt19937 rng(seed);
std::uniform_int_distribution<int> dice(1, 6);
std::array<int, 7> hist{};   /* index 1..6; index 0 unused */

for (int i = 0; i < 1000; ++i)
    ++hist[dice(rng)];
```

The sum of buckets 1-6 is exactly 1000 (every roll lands
somewhere). The individual buckets distribute roughly evenly.

### Determinism for tests

`uniform_int_distribution`'s output is *deterministic per
implementation* given a fixed seed and engine. Tests use a
fixed seed (e.g. 42) and check exact histogram values --
which works *within* a libstdc++ version but might vary
across libstdc++/libc++/MSVC. For portable assertions, test
properties (sum == N, all bucket counts non-zero, within a
range) instead of exact values.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task123
{
    /* Roll a 6-sided die `rolls` times using mt19937 seeded with
     * `seed`. Return a histogram of size 7 (index 1..6;
     * index 0 unused / zero). */
    std::array<int, 7> roll_dice_histogram(std::uint_fast32_t seed, int rolls);

    /* Return one sample from uniform_real_distribution<double>(lo, hi)
     * with mt19937 seeded with `seed`. */
    double uniform_real_sample(std::uint_fast32_t seed, double lo, double hi);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `roll_dice_histogram`:
  ```cpp
  std::mt19937 rng(static_cast<std::mt19937::result_type>(seed));
  std::uniform_int_distribution<int> dice(1, 6);
  std::array<int, 7> hist{};
  for (int i = 0; i < rolls; ++i)
      ++hist[dice(rng)];
  return hist;
  ```

- `uniform_real_sample`:
  ```cpp
  std::mt19937 rng(static_cast<std::mt19937::result_type>(seed));
  std::uniform_real_distribution<double> dist(lo, hi);
  return dist(rng);
  ```

- The histogram is sized 7 to allow direct indexing by the die
  face value (1..6). Index 0 is wasted but the code is
  cleaner. The test verifies index 0 stays at zero.

- Tests check *properties*: sum is correct, all buckets
  non-zero with enough rolls, individual buckets in a range,
  sample is in [lo, hi). Exact values aren't portably
  meaningful across STL versions.

- For 1000 rolls of a fair die, each bucket *should* be near
  167. A 3σ band is roughly 167 ± 35, so any bucket in
  [130, 200] is plausible. The test uses a loose `[120, 220]`
  range to avoid spurious failures.

- C++14 added `std::generate_canonical<T>` for "uniformly
  distributed in [0, 1)" without needing the distribution
  object -- it goes engine-to-double directly. For
  parameterised ranges, the distribution is still the
  cleanest API.
