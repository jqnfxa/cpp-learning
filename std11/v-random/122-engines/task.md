# 122 — engines: `std::mt19937` / `std::mt19937_64`

> Spec: <https://en.cppreference.com/cpp/numeric/random>

## What you learn

C++11's `<random>` header provides **proper random-number
generation** to replace the C-style `std::rand()` / `srand`
duo (deprecated, biased, low entropy). The toolkit splits into
three layers:

| Layer            | Examples                                      | Role                       |
|------------------|-----------------------------------------------|----------------------------|
| **Source**       | `std::random_device`                           | OS entropy                 |
| **Engine**       | `std::mt19937`, `std::mt19937_64`, `std::minstd_rand` | PRNG / deterministic       |
| **Distribution** | `std::uniform_int_distribution`, etc.         | Maps engine output to a distribution |

Step 84 used `std::mt19937` to seed `std::shuffle`. This step
goes one layer up: seeding the engine itself from
`std::random_device`, and generating raw values.

### `std::mt19937` — the Mersenne Twister

```cpp
std::mt19937 rng(seed);          /* fixed-seed: deterministic */
std::uint_fast32_t v = rng();    /* invoke -> raw value */
```

`std::mt19937` produces 32-bit `unsigned` values with period
2^19937 - 1 (hence the name). It's fast, has excellent
statistical properties, and is the standard's default
recommendation for general-purpose use.

Variants:
- `std::mt19937_64`: 64-bit output.
- `std::minstd_rand`: minimal linear congruential. Smaller
  state, faster, much worse quality.
- `std::ranlux24_base`, `ranlux48_base`: bigger state, better
  quality.

For 99% of use cases, `mt19937` is fine.

**Not cryptographically secure.** `mt19937` is deterministic
and predictable from ~624 consecutive outputs. Don't use it
for passwords, session tokens, or anything an attacker might
care about. Use `std::random_device` directly or a
cryptographic library (libsodium, OpenSSL) for that.

### `std::random_device` — OS entropy source

```cpp
std::random_device dev;
std::uint_fast32_t seed = dev();    /* one fresh entropy word */
```

`std::random_device` is the C++11 wrapper around the OS's
entropy source -- on Linux it reads from `/dev/urandom` or
`getrandom(2)`, on Windows it uses `BCryptGenRandom` /
similar. Calls are slow compared to engine output, but each
result is unpredictable.

The classic recipe:

```cpp
std::random_device dev;
std::mt19937 rng(dev());            /* fresh, non-determinism seed */
```

`mt19937` then runs deterministically from that seed, but
the seed itself is unpredictable. For reproducible tests,
pass a fixed seed instead.

### Why two functions in this task?

The step asks for two flavours:

- **`generate_8()`**: use `random_device` to seed `mt19937`,
  then produce 8 values. Each call returns a different
  sequence (extremely high probability).
- **`generate_8_seeded(seed)`**: use the user-supplied seed.
  Two calls with the same seed return *identical* sequences --
  the determinism guarantee.

Pairing these makes the entropy/engine distinction visible.

### Engine output is *raw* — apply a distribution

`rng()` returns a value in
`[std::mt19937::min(), std::mt19937::max()]` (typically `[0,
2^32 - 1]`). To get "uniform in [1, 6]" you need a
**distribution**:

```cpp
std::uniform_int_distribution<int> dice(1, 6);
int roll = dice(rng);
```

Step 123 covers distributions. This step just exposes the
engine output, which is uniform over its native range.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task122
{
    /* Seed an mt19937 from std::random_device and return 8
     * consecutive raw outputs. */
    std::vector<std::uint_fast32_t> generate_8();

    /* Seed an mt19937 with `seed` and return 8 consecutive raw
     * outputs. Two calls with the same seed must return identical
     * sequences. */
    std::vector<std::uint_fast32_t> generate_8_seeded(std::uint_fast32_t seed);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `generate_8`:
  ```cpp
  std::random_device dev;
  std::mt19937 rng(dev());
  std::vector<std::uint_fast32_t> out;
  out.reserve(8);
  for (int i = 0; i < 8; ++i) out.push_back(rng());
  return out;
  ```

- `generate_8_seeded`:
  ```cpp
  std::mt19937 rng(seed);
  std::vector<std::uint_fast32_t> out;
  out.reserve(8);
  for (int i = 0; i < 8; ++i) out.push_back(rng());
  return out;
  ```

- `<random>` is the header. Both `std::random_device` and
  `std::mt19937` live there.

- `std::uint_fast32_t` is the "smallest unsigned integer with
  at least 32 bits that's fast on this platform". On most
  64-bit systems it's a 64-bit type. `mt19937::result_type` is
  guaranteed to be `uint_fast32_t`, so the function signatures
  match.

- For the seed test, the contract is "same seed -> same
  sequence". Implementations may differ across compiler
  versions, but within one binary, two calls with the same
  seed produce identical output. The test relies on this
  internal determinism.

- For the non-determinism test, you can't *prove* that two
  calls to `generate_8()` give different sequences (with
  astronomically small probability they could be equal --
  same seed read from random_device, same sequence). The test
  uses a softer "very probably different" check.

- `std::random_device` is allowed to be deterministic on
  platforms without an entropy source (the standard says so).
  In that case, `generate_8()` *would* be deterministic. The
  tests are written to tolerate this edge case but still
  exercise the seeding plumbing.

- For seeding from multiple words of entropy, the
  `std::seed_seq` adapter is the proper tool. The standard
  spelling `std::seed_seq seq{dev(), dev(), dev(), dev()};
  std::mt19937 rng(seq);` is the production-grade form. For
  this task, single-word seeding is enough to show the
  pipeline.
