# 80 — `std::all_of` / `any_of` / `none_of`

> Spec: <https://en.cppreference.com/cpp/algorithm/all_any_none_of>

## What you learn

These three were added to C++11 as the obvious "predicate over a
range, returns bool" trio:

```cpp
template <class It, class Pred>
bool all_of(It first, It last, Pred pred);

template <class It, class Pred>
bool any_of(It first, It last, Pred pred);

template <class It, class Pred>
bool none_of(It first, It last, Pred pred);
```

Each walks the range and short-circuits as soon as the answer is
decided:

- `all_of`: stops at the first `pred(x) == false`.
- `any_of`: stops at the first `pred(x) == true`.
- `none_of`: stops at the first `pred(x) == true` (returns
  false from there).

### Vacuous truth — what they do on an empty range

| Function    | Empty range result |
|-------------|--------------------|
| `all_of`    | **true**           |
| `any_of`    | false              |
| `none_of`   | **true**           |

Mathematically: "for all x in {}, P(x)" is vacuously true. "There
exists x in {}, P(x)" is false. "No x in {} satisfies P(x)" is
true. C++ matches the maths.

### Why these matter

Three reasons to reach for them over hand-rolled loops:

1. **Intent is in the name.** `all_of` says exactly what it means
   in one read. The for-loop equivalent is six lines you have to
   skim to understand.

2. **Short-circuit is free.** Each algorithm stops at the first
   decisive element. The hand-rolled loop has the same bound but
   the algorithm spelling makes it explicit.

3. **Predicate composition.** Combined with C++11 lambdas,
   `[](int x) { return x > 0; }` is a natural fit. The result is
   a one-liner.

```cpp
bool all_positive = std::all_of(v.begin(), v.end(),
                                [](int x) { return x > 0; });
```

C++20 adds the ranges versions (`std::ranges::all_of(v, pred)`)
which save you the `begin()` / `end()` pair. For C++11 you write
the iterator pair.

## Task

In `solution.hpp`, implement three vector predicates:

```cpp
namespace task80
{
    /* true iff every element of v is > 0.  (true on empty.) */
    bool is_all_positive(const std::vector<int> &v);

    /* true iff at least one element of v is exactly 0.
     * (false on empty.) */
    bool has_zero(const std::vector<int> &v);

    /* true iff no element of v is < 0.  (true on empty.) */
    bool none_negative(const std::vector<int> &v);
}
```

Each is one line:
- `is_all_positive` -> `std::all_of(...)` with `x > 0`.
- `has_zero` -> `std::any_of(...)` with `x == 0`.
- `none_negative` -> `std::none_of(...)` with `x < 0`.

## Run

```sh
./verify.sh
```

## Hints

- Include `<algorithm>` for the three functions.

- The lambda is the predicate -- a unary `int -> bool`. Captures
  are not needed:
  `[](int x) { return x > 0; }`.

- Vacuous-truth tests are part of the suite. `is_all_positive({})`
  is true (no negative or zero element exists to falsify it).
  `has_zero({})` is false. `none_negative({})` is true.

- Don't try to short-circuit by hand. The algorithms already do.
  A for-loop with an explicit `break` is the same code with more
  characters.

- C++11 lambda syntax: `[capture-list](params) -> return-type {
  body }`. For these predicates, no captures, no explicit return
  type. The short form `[](int x) { return x > 0; }` is what you
  want.

- These three round out the predicate-vocabulary of `<algorithm>`
  alongside `find_if`, `count_if`, `remove_if`, etc. — all of
  them take a unary predicate and walk a range. The C++11
  lambda was the missing piece that made them ergonomic.
