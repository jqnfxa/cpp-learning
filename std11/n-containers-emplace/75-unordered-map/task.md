# 75 — `std::unordered_map<K, V>`

> Spec: <https://en.cppreference.com/cpp/container/unordered_map>

## What you learn

`std::unordered_map<K, V>` is C++11's hash-based associative
container. It pairs every key with one value, with **average
O(1)** insert / find / erase — the textbook hash-table.

Compared to `std::map<K, V>` (which is a balanced tree):

| Property                     | `std::map`            | `std::unordered_map`         |
|------------------------------|-----------------------|------------------------------|
| Underlying structure         | Red-black tree        | Hash table (chained buckets) |
| Average operations           | O(log n)              | **O(1)**                     |
| Worst-case operations        | O(log n)              | O(n) (everything one bucket) |
| Iteration order              | Sorted by key         | **Unspecified**              |
| Requires from `K`            | `<` (ordering)        | `hash<K>`, `==`              |
| Memory overhead per element  | Lower                 | Higher (bucket array)        |

Reach for `unordered_map` when:
- You don't need sorted iteration.
- The keys have a good `std::hash` specialisation (or you can
  supply one).
- You want O(1) lookups on average.

Reach for `map` when:
- You need ordered iteration (e.g. for serialisation, range
  queries).
- The keys don't have a sensible hash.

### The `operator[]` gotcha

```cpp
std::unordered_map<std::string, int> m;

if (m["needle"] > 0) { ... }   /* m now has {"needle", 0} in it */
```

`operator[]` **inserts a default-constructed value if the key
isn't already there**, and returns a reference to it. That's
deliberate — it's what makes the idiomatic increment work:

```cpp
++m["apple"];   /* expands to ++m.operator[]("apple")
                   ↓
                 ++ (default-constructed-int already at "apple")
                   ↓
                 m["apple"] is now whatever it was, plus one */
```

But for *read-only* checks, use `find()` or `count()`:

```cpp
auto it = m.find("needle");
if (it != m.end()) { ... }      /* doesn't insert */

if (m.count("needle")) { ... }   /* doesn't insert, returns 0 or 1 */
```

### Iteration order is unspecified

```cpp
for (const auto &kv : m) { ... }   /* order can change between insertions */
```

The standard does not promise any order. Different STL
implementations differ; the same implementation can reorder when
the table rehashes (grows). Never rely on the iteration order
for correctness.

### Hashing for `std::string`

C++11 ships `std::hash<std::string>` specialised in `<string>`,
so `unordered_map<std::string, T>` works out of the box. For
user-defined types you write a specialisation:

```cpp
namespace std
{
    template <>
    struct hash<my_key>
    {
        std::size_t operator()(const my_key &k) const noexcept
        {
            return std::hash<int>()(k.id);
        }
    };
}
```

(Or supply a custom hash and equality as the third and fourth
template arguments to `unordered_map`.)

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task75
{
    using counts = std::unordered_map<std::string, std::size_t>;

    /* Count occurrences of each string in `words`. */
    counts word_count(const std::vector<std::string> &words);

    /* Look up the count for `word`. Returns 0 if not present.
     * Must NOT mutate `m` -- in particular, do not use operator[]. */
    std::size_t count_of(const counts &m, const std::string &word);
}
```

`word_count` is the canonical use: walk the input vector,
`++m[word]` for each one. `count_of` is the *read-only* lookup
that avoids the `operator[]` insertion trap — use `m.find(word)`.

## Run

```sh
./verify.sh
```

## Hints

- `word_count` is two lines if you use the range-for + `++m[w]`
  idiom.

- `count_of` must use `find()` or `count()` -- you cannot use
  `operator[]` here, because `operator[]` requires a non-const
  map (it might insert) and `m` is passed by const ref. The
  type system enforces this; that's a feature.

- The return type of `find()` is `unordered_map::const_iterator`
  on a const map, pointing to a `std::pair<const K, V>`. Use
  `it->second` to get the count.

- Iteration order matters in some tests *only* through
  reconstruction of the multiset of values, never order. Use
  `EXPECT_EQ` on map<-->map comparisons (which compare as sets
  of pairs).

- The default load factor before a rehash is 1.0 (one element
  per bucket on average). The bucket array grows automatically.
  You don't need to call `reserve()` or `rehash()` for this
  task -- the defaults are fine.

- A common interview question: "What's the difference between
  `map` and `unordered_map`?". Be ready with at least: tree vs
  hash, O(log n) vs O(1) avg, sorted vs unspecified iteration,
  requires `<` vs requires hash, and the iterator-invalidation
  difference (rehash on `unordered_map` invalidates iterators;
  `map` doesn't except for the erased element).
