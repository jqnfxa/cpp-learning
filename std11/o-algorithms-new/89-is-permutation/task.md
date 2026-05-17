# 89 — `std::is_permutation`

> Spec: <https://en.cppreference.com/cpp/algorithm/is_permutation>

## What you learn

`std::is_permutation(first1, last1, first2)` tells you whether
the range `[first1, last1)` is a permutation of the range
starting at `first2`. C++11 introduced it; C++14 added a
four-iterator overload that takes the full second range too.

```cpp
std::string a = "listen", b = "silent";
bool same_letters = std::is_permutation(a.begin(), a.end(), b.begin());
/* true -- "listen" and "silent" are anagrams */
```

It's the right tool for "are these two collections equal as
multisets?". The element types only need `==` for matching;
ordering is irrelevant.

### Cost

The complexity is up to O(n²) in the worst case for the C++11
3-iterator overload (no special ordering assumed). Modern STL
implementations short-circuit and avoid full O(n²) work for
many common inputs, but the worst case stands.

For large ranges, you might prefer sort-both-then-compare
(O(n log n)) or a hash-based counter (average O(n)). For string
anagrams over a fixed alphabet (e.g. ASCII / Latin-1), a
**character-frequency array** beats both.

### The C++11 3-iterator pitfall

```cpp
std::is_permutation(first1, last1, first2);
```

This overload assumes the second range has at least as many
elements as `last1 - first1`. **If the second range is shorter,
the algorithm reads past its end -- undefined behaviour.**

The conventional fix when you have two `std::string`s of
unknown lengths:

```cpp
bool is_anagram(const std::string &a, const std::string &b)
{
    if (a.size() != b.size()) return false;
    return std::is_permutation(a.begin(), a.end(), b.begin());
}
```

C++14 added `is_permutation(first1, last1, first2, last2)`
which takes both ends and handles the length check itself —
preferred whenever it's available.

### Case and whitespace

`std::is_permutation` compares with `==` by default. `'a' == 'A'`
is false in C++. If you want case-insensitive anagram
detection, normalise both inputs first (e.g. lowercase via
`std::transform` + `std::tolower(unsigned char)`) — *don't*
pass a "case-insensitive" comparator and hope, because the
algorithm's complexity bound only holds for an equivalence
relation, which "case-insensitive equality" is.

For this task we keep it simple: literal character equality.

## Task

In `solution.hpp`, implement one function:

```cpp
namespace task89
{
    /* True iff `a` and `b` contain exactly the same multiset of
     * characters -- i.e. they are anagrams (with case and whitespace
     * treated literally). Returns false on length mismatch. */
    bool is_anagram(const std::string &a, const std::string &b);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The check is two lines:
  ```cpp
  if (a.size() != b.size()) return false;
  return std::is_permutation(a.begin(), a.end(), b.begin());
  ```

- The length check is **not optional** in C++11. Without it,
  `is_permutation` over `("abc", "ab")` reads past the second
  range's end. The size guard is what makes the call safe.

- `std::is_permutation` is in `<algorithm>`. Include it.

- The comparator defaults to `==`. Characters are compared as
  `char`, which means case-sensitive ASCII. "Listen" and
  "Silent" with capital initials are **not** anagrams under
  this definition.

- For the typical "is this an anagram" interview question, the
  three classic approaches are:
  - Sort both, then compare (O(n log n), needs a copy or
    mutation).
  - Hash-count the letters in one, decrement for the other
    (average O(n)).
  - For ASCII, a 256-int array of frequencies (O(n) flat).

  `std::is_permutation` is the off-the-shelf O(n²)-worst-case
  option that doesn't require you to copy or hash. It's not
  always the fastest; it *is* the shortest.

- C++14's 4-iterator `is_permutation` removes the length-
  precondition entirely. If you're on C++14 or later, prefer
  `std::is_permutation(a.begin(), a.end(), b.begin(), b.end())`.
  This task stays in C++11 for the explicit length-check
  pedagogy.
