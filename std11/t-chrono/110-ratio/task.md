# 110 — `std::ratio`

> Spec: <https://en.cppreference.com/cpp/numeric/ratio>

## What you learn

`std::ratio<Num, Den>` is a **compile-time exact rational
number**. It's a class template whose only role is to encode a
fraction in the type system:

```cpp
std::ratio<1, 2>          /* 1/2 */
std::ratio<3, 7>          /* 3/7 */
std::ratio<60, 1>          /* 60/1 = 60 (an integer) */
```

The numerator and denominator are accessible as static
`constexpr` members:

```cpp
std::ratio<3, 7>::num     /* 3  */
std::ratio<3, 7>::den     /* 7  */
```

Ratios are **automatically reduced** to lowest terms:

```cpp
std::ratio<2, 4>::num     /* 1 (not 2) */
std::ratio<2, 4>::den     /* 2 (not 4) */
std::ratio<-3, 6>::num    /* -1 */
std::ratio<-3, 6>::den    /* 2 */
```

Sign is normalised onto the numerator: `std::ratio<3, -6>::num
== -1`, `std::ratio<3, -6>::den == 2`.

### Why does this exist?

Two reasons, neither obvious from the type name:

1. **The `Period` of `std::chrono::duration`.** Every duration
   type has a `std::ratio` as its tick-period:
   - `std::chrono::seconds`     -> `std::ratio<1>`
   - `std::chrono::milliseconds` -> `std::ratio<1, 1000>`
   - `std::chrono::minutes`     -> `std::ratio<60>`
   - `std::chrono::hours`       -> `std::ratio<3600>`

   The compile-time conversion between units is implemented as
   arithmetic on these ratios.

2. **Pre-defined SI prefixes** for any kind of "scaled
   quantity":
   - `std::milli`  = `ratio<1, 1000>`
   - `std::micro`  = `ratio<1, 1000000>`
   - `std::nano`   = `ratio<1, 1000000000>`
   - `std::kilo`   = `ratio<1000>`
   - `std::mega`   = `ratio<1000000>`
   - etc.

### Compile-time arithmetic

The standard library provides four operations on ratios:

```cpp
std::ratio_add<R1, R2>          /* addition */
std::ratio_subtract<R1, R2>      /* subtraction */
std::ratio_multiply<R1, R2>      /* multiplication */
std::ratio_divide<R1, R2>        /* division */
```

Each is itself a `std::ratio<num, den>` (already reduced). For
example:

```cpp
std::ratio_add<std::ratio<1, 3>, std::ratio<1, 6>>
/* == std::ratio<1, 2> -- (2 + 1) / 6 = 3/6, reduced to 1/2 */
```

And four comparison metafunctions:

```cpp
std::ratio_equal<R1, R2>::value          /* R1 == R2 */
std::ratio_not_equal<R1, R2>::value
std::ratio_less<R1, R2>::value
std::ratio_less_equal<R1, R2>::value
std::ratio_greater<R1, R2>::value
std::ratio_greater_equal<R1, R2>::value
```

All of these are **purely compile-time**. There's no runtime
overhead -- the arithmetic happens in the compiler, not at
program execution.

### Why not just use `constexpr double`?

Two reasons:

1. **Exactness.** `1/3 + 1/6 = 1/2` exactly in rationals. The
   same calculation in `double` would be `0.333... + 0.1666... =
   0.4999...`, off by an ulp. Ratios never have that error.

2. **Type-level identity.** Two ratios are *the same type* iff
   they have the same reduced form. This is what lets
   `std::chrono::milliseconds` and `std::chrono::microseconds` be
   distinguishable in the type system. With `double`, they'd
   collapse to the same runtime numeric value.

### Validation via `static_assert`

`std::ratio` arithmetic happens at compile time. The natural
way to validate it is `static_assert`:

```cpp
static_assert(std::ratio_equal<
    std::ratio_add<std::ratio<1, 3>, std::ratio<1, 6>>,
    std::ratio<1, 2>>::value, "1/3 + 1/6 == 1/2");
```

`static_assert` failing is a build error; passing is silent.
That's exactly what you want for math you proved on paper.

## Task

In `solution.hpp`, define one alias:

```cpp
namespace task110
{
    /* The sum of 1/3 and 1/6, computed via std::ratio_add. */
    using one_half = ...;
}
```

The expected value is `std::ratio<1, 2>` (since `1/3 + 1/6 ==
1/2`). Tests use `static_assert(std::ratio_equal<one_half,
std::ratio<1, 2>>::value, ...)`.

## Run

```sh
./verify.sh
```

## Hints

- One line:
  ```cpp
  using one_half = std::ratio_add<std::ratio<1, 3>,
                                   std::ratio<1, 6>>;
  ```

- `std::ratio_add` (and friends) live in `<ratio>`. Include it.

- The result is *already reduced*: `2/6 + 1/6 = 3/6`, which
  `ratio_add` reduces to `1/2`. You don't reduce by hand.

- The standard's metafunction style follows the
  "trait-name<...>::value or ::type" convention. C++14 added
  `_v` and `_t` aliases for many traits; `<ratio>` doesn't get
  those shortcuts -- `std::ratio_add` *is* a type alias (using
  the C++11 "all members" expansion), so you write
  `std::ratio_add<...>` directly without `::type`.

- `std::ratio_equal<R1, R2>::value` is the boolean comparison.
  C++17 adds the `_v` shortcut here too.

- Reading the trait names: `std::ratio_add<A, B>` *is itself* a
  `std::ratio`. You can chain:
  `std::ratio_add<std::ratio_add<A, B>, C>` works for a 3-way
  sum, etc.

- The runtime smoke test exists only so `ctest` has something
  to run -- the validation is the `static_assert` chain.

- A nice exercise extension: write `std::ratio_add<std::milli,
  std::micro>` and confirm it equals `std::ratio<1001, 1000000>`
  (1 millisecond + 1 microsecond = 1.001 milliseconds in exact
  rationals).
