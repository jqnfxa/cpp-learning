# 131 — `std::hash<T>` specialization

> Spec: <https://en.cppreference.com/cpp/utility/hash>

## What you learn

`std::hash<T>` is the standard library's **hash function
object** template. The STL pre-specializes it for primitive
types (`int`, `double`, `std::string`, `pointer`, etc.) and
uses it in every hash-based container: `unordered_set`,
`unordered_map`, `unordered_multiset`, `unordered_multimap`.

For your own value types, you have to **specialize**
`std::hash<your_type>` before they'll work with those
containers (or pass a custom hasher explicitly as the second
template argument).

```cpp
struct point { int x; int y; };

bool operator==(const point&, const point&);   /* required too */

namespace std
{
    template <>
    struct hash<point>
    {
        std::size_t operator()(const point& p) const noexcept
        {
            std::size_t h = std::hash<int>{}(p.x);
            h ^= std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
}

std::unordered_set<point> s;
s.insert({1, 2});
```

### The contract

`std::hash` must satisfy two properties:

1. **Deterministic**: `hash(a)` always returns the same value
   in a single program run.
2. **Equality-preserving**: `a == b` implies `hash(a) == hash(b)`.

The reverse is NOT required: `hash(a) == hash(b)` doesn't
imply `a == b`. That's a collision -- legal and unavoidable.

A "good" hash also satisfies:

3. **Well-distributed**: distinct inputs map to wildly
   different outputs, so bucket distribution in unordered
   containers stays balanced.

Property 1 means hashes can be cached. Property 2 means the
container can use `hash(x) == hash(y)` as a fast pre-check
before the expensive `==` comparison.

### Why specialization, not overload

`std::unordered_set<T>` is declared:

```cpp
template <typename Key,
          typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          ...>
class unordered_set;
```

The default hasher is `std::hash<Key>`. If `Key = point` and
you didn't specialize `std::hash<point>`, instantiation looks
up `std::hash<point>::operator()` -- and fails (or worse, it
finds the un-specialized primary template, which deletes its
operator() and gives an opaque error).

You have **two** options:

1. Specialize `std::hash<point>` in the `std` namespace
   (legal -- the standard allows specializing standard library
   templates for user types).
2. Define your own hasher type and pass it explicitly:
   ```cpp
   struct point_hash { std::size_t operator()(const point&) const; };
   std::unordered_set<point, point_hash> s;
   ```

Option 1 is the textbook approach when `point` is "your" type
and you want `unordered_set<point>` to "just work".

### Why `operator==` is also required

`unordered_set` uses `hash` to find the bucket and then `==`
to find the actual element inside the bucket. Without `==`,
the container can find the bucket but can't tell if your key
is already there. The default for `KeyEqual` is
`std::equal_to<Key>`, which calls `==` -- so you must define
`operator==(const point&, const point&)`.

### Combining hashes: the Boost trick

The Boost library popularized this hash_combine formula:

```cpp
h ^= other + 0x9e3779b9 + (h << 6) + (h >> 2);
```

Where `0x9e3779b9` is the fractional part of the golden ratio
(in 32-bit). This mixes bits well enough for most workloads
and is the de facto standard for "combine two hashes".

A naive `return h1 ^ h2` is BAD: `hash(point{1, 2}) ==
hash(point{2, 1})` because XOR is commutative. A naive
`return h1 + h2` is just as bad -- addition is commutative
too. The Boost formula breaks the symmetry by shifting `h`
before mixing in `other`.

### The pathological all-zero hash

Returning `0` for every input is **technically a valid hash**
(deterministic, equality-preserving). But it puts every
element in the same bucket and degrades `unordered_set` to
O(n) lookups. This is what the scaffold ships with -- the
tests detect it via "two distinct points must hash to
different values, usually".

The "usually" is interesting: every hash CAN collide, by the
pigeonhole principle. The tests use cases where a correct
hash_combine won't collide (small adjacent integers), so the
all-zero stub is clearly wrong.

### Where in the namespace?

The specialization must live in `namespace std`:

```cpp
namespace std
{
    template <>
    struct hash<my_type>
    {
        std::size_t operator()(const my_type&) const noexcept;
    };
}
```

This is one of a few cases where the standard explicitly
permits adding things to `namespace std` -- specializing
existing templates for user-defined types is legal. Adding
new declarations (functions, classes) is not.

A common alternative: define a hasher type in YOUR namespace,
and pass it as the second template argument to the
container. That keeps everything in your namespace, but you
lose the "default works automatically" benefit.

### Const + noexcept

`operator()` should be:
- `const`: it doesn't modify the hasher.
- `noexcept`: hashing should not throw -- the container can
  optimize allocations if it knows hashing is exception-safe.

## Task

In `solution.hpp`, implement `std::hash<task131::point>::operator()`
so distinct points hash to different values (using
hash_combine), while equal points hash to the same value.

The scaffold ships with `operator==(point, point)` already
defined. The TODO is just the body of the hash specialization.

```cpp
namespace std
{
    template <>
    struct hash<task131::point>
    {
        std::size_t operator()(const task131::point& p) const noexcept;
    };
}
```

## Run

```sh
./verify.sh
```

## Hints

- The Boost hash_combine formula:
  ```cpp
  std::size_t h = std::hash<int>{}(p.x);
  h ^= std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
  return h;
  ```

- DO NOT write `return p.x ^ p.y;`. It hashes {1,2} and {2,1}
  to the same value -- the xy-swap test will fail.

- DO NOT write `return p.x + p.y;`. Same problem: commutative.

- `std::hash<int>` is pre-specialized. You can reuse it on the
  two fields and just need a combiner.

- `0x9e3779b9` (2^32 / golden_ratio) is the magic constant
  Boost uses. On 64-bit platforms there's a 64-bit variant
  `0x9e3779b97f4a7c15` for slightly better mixing, but the
  32-bit constant works fine for two ints.

- The specialization MUST live in `namespace std`. The header
  brackets that block for you.

- `operator()` should be `const noexcept`. The scaffold
  already declares it that way.

- `unordered_set` uses BOTH `std::hash<point>` AND
  `operator==(point, point)`. The scaffold defines `==` for
  you; you only fill in `hash`.

- Tests verify: equality (sanity), equal-points-hash-equal
  (the hash contract), set-correctly-deduplicates,
  set-find-works, the THREE "distinct points hash differently"
  cases (xy_swap, origin-vs-nearby, 100-point spread), erase
  works, and a 1000-element stress test.

- An "unordered_set of point" in the wild needs `std::hash<point>`
  to be findable at the point of instantiation. Including the
  specialization header before the `unordered_set` declaration
  is the standard rule. (Headers solve this organically.)
