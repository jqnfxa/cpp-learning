# 24 — Removed: `std::random_shuffle`

`std::random_shuffle` was removed in C++17. Use
`std::shuffle(first, last, engine)` with an explicit random
engine instead.

```cpp
std::mt19937 rng(seed);
std::shuffle(v.begin(), v.end(), rng);
```

The C++03 form used a global RNG that wasn't thread-safe and
had poor distribution. C++11's `std::shuffle` takes an
explicit engine; C++17 removed the old form entirely.

## Hints

- `std::mt19937` is the Mersenne Twister engine.
- For non-deterministic seeding: `std::random_device{}()`.
