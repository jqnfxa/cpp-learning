# 76 — `std::unordered_set<T>`

> Spec: <https://en.cppreference.com/cpp/container/unordered_set>

## What you learn

`std::unordered_set<T>` is the value-set analogue of
`std::unordered_map<K, V>`: a hash-based container that holds at
most one of each value. Average O(1) `insert` / `find` / `erase`.
Unspecified iteration order. Requires `std::hash<T>` and
`operator==` on `T`.

You use it when you only need *membership* — "have I seen this
before?", "is this in the allow-list?". Use `unordered_map` when
you need *values keyed on something*. They share the same
underlying machinery; in fact most implementations have
`unordered_set<T>` as a thin wrapper around the table that backs
`unordered_map`.

### The two methods you mostly use

```cpp
std::unordered_set<int> seen;

auto [it, inserted] = seen.insert(42);   /* (it, true) on first add */
                                          /* (it, false) on duplicate */

if (seen.count(42))     { ... }           /* 0 or 1; returns size_t */
if (seen.find(42) != seen.end()) { ... }  /* same idea, returns iterator */
```

`insert` returns a `pair<iterator, bool>`. The `bool` is *true*
iff a new element was added — which is exactly the "have I seen
this before?" answer. That's the structured-binding-style usage
in C++17; in C++11 you'd write `auto p = seen.insert(42);
if (!p.second) ...`.

### The dedup pattern

Combining `insert`'s return with a walk over an input sequence
gives you a clean "deduplicate preserving first-seen order"
algorithm:

```cpp
std::vector<int> dedup(const std::vector<int> &in)
{
    std::unordered_set<int> seen;
    std::vector<int> out;
    out.reserve(in.size());

    for (int x : in)
    {
        if (seen.insert(x).second)   /* true iff x wasn't there before */
        {
            out.push_back(x);
        }
    }
    return out;
}
```

Order *of the output* is the *first-seen order from the input*.
This is one of the most common uses of `unordered_set`.

Note that `std::sort(v.begin(), v.end()); v.erase(std::unique(...), v.end());`
is the classic "sort + unique" idiom, but it *destroys input
order*. When order matters, the set-based pattern is the right
tool.

### Hashing for `T`

`int`, `std::string`, pointers, and the other obvious types ship
with `std::hash` specialisations. For your own struct, you
either:

1. Specialise `std::hash<my_t>` in namespace `std`.
2. Pass a custom hash type as the second template argument:
   `unordered_set<my_t, my_hash, my_equal>`.

`unordered_set` also needs `operator==` on `T`. Equality is
load-bearing in the chained-bucket walk on collision.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task76
{
    /* Return the elements of `in`, in first-seen order, with later
     * duplicates removed. */
    std::vector<int> dedup(const std::vector<int> &in);

    /* Build an unordered_set containing all elements of `in`. */
    std::unordered_set<int> set_of(const std::vector<int> &in);
}
```

`dedup` is the canonical first-seen-order deduplication. `set_of`
is the simpler "throw everything in a set" -- order is lost, but
duplicates collapse and you get O(1) membership testing.

## Run

```sh
./verify.sh
```

## Hints

- For `dedup`, use the `insert(x).second` trick. It's both faster
  than checking `count(x)` first (one hash + lookup instead of
  two) and more readable.

- `out.reserve(in.size())` before the loop is the standard
  optimisation: at worst we push `in.size()` elements, so
  pre-reserving avoids reallocation.

- `set_of` is one line:
  `return std::unordered_set<int>(in.begin(), in.end());`.
  The range constructor of `unordered_set` does the dedup
  internally.

- `unordered_set::insert` returns `std::pair<iterator, bool>`.
  Until C++17, you don't get structured bindings, so write
  `auto result = s.insert(x); if (result.second) ...` or chain
  `if (s.insert(x).second) ...` directly.

- Iterator invalidation: `insert` *can* rehash the table, which
  invalidates all iterators (but **not** references to elements
  that already existed). Don't hold iterators across inserts.
  `unordered_set` and `unordered_map` share this rule.

- Counting unique values is `set_of(v).size()`. Counting unique
  values in O(1) per element is what makes hash sets the right
  tool over `std::set<T>` (which would be O(log n) per insert).

- Modern uses of `unordered_set` outside dedup: visited-node
  tracking in graph traversal, candidate-set filtering before
  expensive operations, fast set difference / intersection
  helpers.
