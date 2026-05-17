# 2 — Structured bindings: tuples and pairs

> Spec: <https://en.cppreference.com/cpp/language/structured_binding>

## What you learn

The second structured-binding form: **tuple-like types**.
Any type that specializes `std::tuple_size`, `std::tuple_element`,
and `std::get<I>` can be destructured with `auto [a, b, ...]`:

```cpp
std::pair<int, std::string> p{1, "x"};
auto [k, v] = p;          /* k=1, v="x" -- via get<0>, get<1> */

std::tuple<int, double, char> t{1, 2.5, 'c'};
auto [a, b, c] = t;       /* via get<0>, get<1>, get<2> */
```

Structured-binding then "just works" for:
- `std::pair`
- `std::tuple`
- `std::array`
- `std::variant`'s `get` doesn't fit (variant is its own thing)
- Any user type that specializes the tuple-like protocol.

### The motivating use case: std::map::insert

`std::map::insert` returns `std::pair<iterator, bool>`. The
bool says whether the insert happened (true) or the key
already existed (false).

C++14 pattern:

```cpp
auto result = m.insert({k, v});
if (result.second) {
    /* newly inserted */
}
```

`result.first` is "the iterator", `result.second` is "did
insert happen?" -- neither name conveys the meaning.

C++17 structured binding:

```cpp
auto [it, inserted] = m.insert({k, v});
if (inserted) {
    /* it points to the new element */
}
```

The names ARE the meaning. Massive readability win for
this single API alone, never mind everything else.

### Combining with `if`-init-statement (step 4)

C++17 also lets you put the destructuring inside an `if`:

```cpp
if (auto [it, inserted] = m.insert({k, v}); inserted) {
    /* use it */
}
```

This scopes the destructured names to the `if` body. Step 4
covers `if`-init-statement in detail.

### Range-for + structured binding

The other huge ergonomic win: iterating a map.

```cpp
std::map<int, std::string> m;
for (auto const& [k, v] : m) {
    std::cout << k << "=" << v << "\n";
}
```

Each iteration yields a `pair<const int, std::string>`, and
the structured binding splits it into `k` and `v`. Without
this, you'd write `entry.first` and `entry.second` -- legal
but ugly.

### Tuple-like protocol details

For a type `T` to support tuple-like structured binding, it
must specialize three things:

```cpp
namespace std {
    template <>
    struct tuple_size<T> : std::integral_constant<std::size_t, N> {};

    template <std::size_t I>
    struct tuple_element<I, T> { using type = ...; };
}

template <std::size_t I>
auto get(T& t) { ... }       /* or a member t.get<I>() */
```

`std::pair`, `std::tuple`, and `std::array` all do this for
you. For user types, you'd add the specializations to make
your type structured-binding-compatible -- but step 3 shows
that for plain structs, the compiler does it automatically
via the third (struct) form.

### `auto` vs `auto&` vs `auto&&`

Same rules as arrays (step 1):

```cpp
auto [a, b] = make_pair(1, 2);          /* a, b are copies (int) */
auto& [a, b] = some_pair;               /* a, b alias the pair's elements */
auto const& [a, b] = some_pair;         /* a, b are const int& */
auto&& [a, b] = some_function();        /* perfect-forwards (rare) */
```

The qualifier applies to the WHOLE binding -- you can't have
`auto a` and `auto& b` from the same source.

### Reading a `pair<const int, V>` for map elements

Map elements are `pair<const Key, Value>`. The `const` on the
key is part of the type, so:

```cpp
for (auto& [k, v] : m) {
    k = 999;        /* error -- k is `const int&` */
    v = "x";        /* OK -- v is non-const */
}
```

This is desirable: you can update map values but not keys
(which would corrupt the map's internal ordering).

### The actual mechanism

Structured binding via tuple-like is equivalent to:

```cpp
auto e = expr;                                /* materialize the source */
auto& a = std::get<0>(e);
auto& b = std::get<1>(e);
```

The compiler synthesizes that. The names `a`, `b` refer to
the same storage that `get<I>(e)` would access.

For your own types: specialize `tuple_size` / `tuple_element` /
provide `get` (or member `get`) and you're done. Most code
doesn't need this -- structs (step 3) auto-deduce.

### Number of bindings must match

```cpp
std::pair<int, int> p;
auto [a, b]    = p;     /* OK */
auto [a]       = p;     /* error: arity mismatch */
auto [a, b, c] = p;     /* error: arity mismatch */
```

Same rule as arrays. Get the count right.

### What about `std::tie`?

`std::tie` was the C++11 alternative:

```cpp
int k;
bool inserted;
std::tie(k_iter, inserted) = m.insert({k, v});   /* ugly */
```

Drawbacks: must declare the variables beforehand, can't use
`auto`, the variables must be default-constructible. C++17's
structured binding replaces `std::tie` for the destructure-
on-declaration case.

## Task

In `solution.hpp`, fill in four functions:

```cpp
inline bool try_insert(std::map<int, int>& m, int k, int v) {
    auto [it, inserted] = m.insert({k, v});
    (void)it;
    return inserted;
}

inline int pair_sum(std::pair<int, int> p) {
    auto [a, b] = p;
    return a + b;
}

inline std::string tuple_summary(std::tuple<int, std::string, double> t) {
    auto [age, name, score] = t;
    (void)score;
    return name + ":" + std::to_string(age);
}

inline double tuple_score(std::tuple<int, std::string, double> t) {
    auto [age, name, score] = t;
    (void)age;
    (void)name;
    return score;
}
```

## Run

```sh
./verify.sh
```

## Hints

- `m.insert({k, v})` returns
  `std::pair<std::map<...>::iterator, bool>`. The `[it,
  inserted]` form gives the two fields meaningful names.

- The `try_insert` function's stub returns false unconditionally
  without modifying the map. The reference inserts and
  returns the bool.

- For `tuple_summary`, the format is `"name:age"` (string
  field, colon, int field as string).

- `std::to_string(int)` for converting the age. Already
  available via `<string>`.

- `auto [age, name, score] = t;` makes three local copies.
  For tuples of small types this is fine; for large types,
  `auto& [...]` aliases.

- The range-for + binding test exercises a common pattern:
  ```cpp
  for (auto const& [k, v] : m) { ... }
  ```
  This is THE C++17 way to iterate a map.

- `(void)variable;` silences "unused parameter" warnings
  -- you destructure all three fields but only use one or
  two.

- Tests verify: insert true/false, multiple insertions,
  pair_sum simple/negative/zero, tuple_summary for alice and
  bob, tuple_score for an arbitrary value, direct binding
  (no helper function), and range-for + binding.

- The C++17 tuple-like form is THE biggest "this just makes
  sense now" feature for everyday C++. Use it everywhere
  you previously wrote `.first` / `.second` or `std::get<I>`.
