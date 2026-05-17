# 84 — `std::shuffle`

> Spec: <https://en.cppreference.com/cpp/algorithm/random_shuffle>

## What you learn

`std::shuffle` is the C++11 replacement for the C++03
`std::random_shuffle`. The difference is the **source of
randomness**:

| Function                 | C++ era | Randomness source             |
|--------------------------|---------|-------------------------------|
| `std::random_shuffle`    | C++03   | Implementation-defined; `std::rand()` by default; **deprecated in C++14, removed in C++17** |
| `std::shuffle`           | C++11   | A *Uniform Random Bit Generator* you supply — usually `std::mt19937` |

The motivation is twofold:

1. **Quality of the randomness.** `std::rand()` is famously bad
   (low entropy, short period, biased modulus). `std::mt19937`
   (Mersenne Twister) is a proper PRNG with a long period and
   uniform output.

2. **Reproducibility.** You pass the engine in, you control the
   seed, you get a deterministic shuffle. That's what unit tests
   want.

```cpp
std::mt19937 rng(42);
std::vector<int> v{1, 2, 3, 4, 5};
std::shuffle(v.begin(), v.end(), rng);
```

### Anatomy of a C++11 random pipeline

```cpp
/* 1. Pick (or fix) a seed. */
std::random_device device;        /* OS entropy source */
std::mt19937 rng(device());        /* or rng(42) for reproducibility */

/* 2. Optionally wrap in a distribution. */
std::uniform_int_distribution<int> dist(0, 51);
int card = dist(rng);

/* 3. Or use the engine directly with algorithms. */
std::shuffle(deck.begin(), deck.end(), rng);
```

For `std::shuffle` you hand the **engine** directly (not a
distribution). The algorithm uses internal `uniform_int`-like
arithmetic to pick swap indices.

### Why `mt19937`

`std::mt19937` is the 32-bit Mersenne Twister: period 2¹⁹⁹³⁷ - 1,
good statistical properties for non-cryptographic use, fast
enough for almost everything. The `_64` variant uses 64-bit
output. Both are seedable, copyable, and serialisable.

Don't use for cryptography. `mt19937` is *deterministic* in the
mathematical sense — you can predict future outputs from
~624 consecutive 32-bit observations. C++11 introduced
`<random>` for simulation, games, and statistics, not security.

### Determinism caveat across STL implementations

`std::shuffle` *seeded with the same engine* gives a reproducible
permutation *within one implementation*. Different STL vendors
implement the swap loop differently, so the same `(seed, deck)`
pair can produce **different** permutations on libstdc++ vs
libc++ vs MSVC.

For this reason, the tests in this step *don't* pin a specific
"the cards must be in this exact order" sequence — they pin
**properties**: deterministic-with-same-seed, varies-with-
different-seed, output is a permutation of the input.

## Task

In `solution.hpp`, implement three functions:

```cpp
namespace task84
{
    using deck = std::vector<int>;

    /* Return a fresh 52-card deck (values 0..51 in ascending order). */
    deck make_deck();

    /* Shuffle d in place using rng. */
    void shuffle_deck(deck &d, std::mt19937 &rng);

    /* Build a deck, shuffle it with rng, return the first 5 cards. */
    std::vector<int> deal_five(std::mt19937 &rng);
}
```

`make_deck()` is one loop. `shuffle_deck` is one call to
`std::shuffle`. `deal_five` composes them and slices.

## Run

```sh
./verify.sh
```

## Hints

- `make_deck` is a simple `for (int i = 0; i < 52; ++i)` loop
  filling a `std::vector<int>` of size 52. Or `std::iota(d.begin(),
  d.end(), 0)` from `<numeric>` -- either way.

- `shuffle_deck` is one line:
  `std::shuffle(d.begin(), d.end(), rng);`. Take the engine by
  non-const reference so the algorithm can advance it.

- `deal_five` is three lines: make a deck, shuffle it, return a
  fresh `std::vector<int>(d.begin(), d.begin() + 5)`.

- Don't seed inside the functions. Tests need to control the
  seed; they pass the engine in. Each call advances the engine,
  which is the point.

- `std::shuffle` requires a *Uniform Random Bit Generator*
  (URBG). `std::mt19937` is one. Don't try to pass `std::rand`
  or a stateless generator wrapper.

- The C++14 deprecation note: `std::random_shuffle` is gone in
  C++17. Always use `std::shuffle` in new code.

- The Fisher-Yates algorithm is what `std::shuffle` implements
  (or is allowed to implement — the standard doesn't mandate
  the exact algorithm). The end result is the same: uniform
  random permutation.
