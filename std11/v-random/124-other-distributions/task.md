# 124 — distributions: `normal`, `bernoulli`, `poisson`

> Spec: <https://en.cppreference.com/cpp/numeric/random>

## What you learn

C++11's `<random>` ships a generous set of distributions
beyond uniform_int / uniform_real. The three most common
non-uniform ones:

| Distribution | Parameters | Analytic mean |
|--------------|------------|---------------|
| `std::normal_distribution<double>(mu, sigma)`   | mean, stddev | mu       |
| `std::bernoulli_distribution(p)`                | success prob | p        |
| `std::poisson_distribution<int>(lambda)`        | rate         | lambda   |

Each is a callable that takes an engine and returns one draw:

```cpp
std::mt19937 rng(42);
std::normal_distribution<double>   normal(0.0, 1.0);
std::bernoulli_distribution        flip(0.5);
std::poisson_distribution<int>     pois(5.0);

double x  = normal(rng);   /* ~N(0, 1) */
bool   b  = flip(rng);     /* true with probability 0.5 */
int    k  = pois(rng);     /* Poisson(5) -- usually 3-8 */
```

### `normal_distribution<RealType>`

Generates `RealType` values from the normal (Gaussian)
distribution. The implementation uses the polar (Marsaglia) or
Box-Muller method internally. Cheap to construct, cheap to
sample.

The 68-95-99.7 rule: 68% within ±1σ, 95% within ±2σ, 99.7%
within ±3σ. For `normal(0, 1)`, that's most samples in
[-1, 1], few in [-3, 3], near-zero outside.

### `bernoulli_distribution`

A single coin flip. Returns `bool`; `true` with probability
`p`, `false` with probability `1 - p`. The simplest non-
uniform distribution: one parameter, one bit out.

For sampling stats:

```cpp
int trues = 0;
for (int i = 0; i < 1000; ++i) if (flip(rng)) ++trues;
double sample_p = trues / 1000.0;     /* ~ p */
```

### `poisson_distribution<IntType>`

Counts of events in a fixed interval. Parameter `lambda` is
both the mean and the variance.

Standard use: model arrivals (customers, packets, photons) in
a unit of time. With λ = 5 you'd expect 5 arrivals on
average; values range from 0 to "much larger than λ" with
exponentially decaying probability.

### Sample mean → analytic mean (law of large numbers)

The fundamental test for any distribution: draw many samples
and average them. The sample mean converges to the analytic
mean.

For N draws from a distribution with mean μ and variance σ²,
the sample mean has standard error σ / √N. With N = 1000:

- `normal(0, 1)`: SE = 1/√1000 ≈ 0.032. Sample mean in
  ±3σ_SE = ±0.10 with 99.7% probability.
- `bernoulli(0.5)`: SE = √(0.25/1000) ≈ 0.016. Sample proportion
  in ±0.05.
- `poisson(5)`: SE = √(5/1000) ≈ 0.071. Sample mean in ±0.21.

The tests use loose tolerances to avoid flaky failures on the
tails of the distribution.

### Why these three?

- **normal**: the workhorse of statistics. Anything continuous
  with a peak.
- **bernoulli**: one-shot binary outcomes. The building block
  of binomial (binomial = N bernoullis) and binary classifiers.
- **poisson**: counting rare events. Bug reports per hour,
  raindrops on a square, particle decays.

Knowing how to sample from each is the foundation for any
Monte Carlo work in C++.

## Task

In `solution.hpp`, implement three functions:

```cpp
namespace task124
{
    /* Sample n draws from N(mean, stddev). Return the sample mean. */
    double normal_mean(std::uint_fast32_t seed, int n,
                       double mean, double stddev);

    /* Sample n draws from Bernoulli(p). Return the sample proportion
     * of successes. */
    double bernoulli_mean(std::uint_fast32_t seed, int n, double p);

    /* Sample n draws from Poisson(lambda). Return the sample mean. */
    double poisson_mean(std::uint_fast32_t seed, int n, double lambda);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `normal_mean`:
  ```cpp
  std::mt19937 rng(seed);
  std::normal_distribution<double> dist(mean, stddev);
  double sum = 0;
  for (int i = 0; i < n; ++i) sum += dist(rng);
  return sum / n;
  ```

- `bernoulli_mean` returns a double in [0, 1]:
  ```cpp
  std::mt19937 rng(seed);
  std::bernoulli_distribution dist(p);
  int hits = 0;
  for (int i = 0; i < n; ++i) if (dist(rng)) ++hits;
  return double(hits) / n;
  ```

- `poisson_mean`:
  ```cpp
  std::mt19937 rng(seed);
  std::poisson_distribution<int> dist(lambda);
  long long sum = 0;
  for (int i = 0; i < n; ++i) sum += dist(rng);
  return double(sum) / n;
  ```

- `std::poisson_distribution` defaults to `IntType = int`.
  Sum into `long long` to be safe against very large λ; for
  λ = 5, sums fit easily in `long`.

- The tests use n = 1000 and check sample mean within a 3σ_SE
  loose band. The exact band per distribution:
  - normal(0, 1): mean in [-0.15, 0.15]
  - bernoulli(0.5): prop in [0.40, 0.60]
  - poisson(5): mean in [4.5, 5.5]

  The loose bands tolerate flaky-on-the-tails outliers; the
  exact-seed determinism means the test result is the same
  every run within one STL implementation.

- For testing, fix the seed so the result is reproducible.
  In production you'd use random_device, but tests need
  determinism.

- C++20 added `std::ranges::generate` which makes sampling
  N draws into a vector a one-liner. The C++11 form here uses
  an explicit loop.
