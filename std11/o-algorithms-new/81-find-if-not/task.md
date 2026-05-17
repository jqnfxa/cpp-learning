# 81 — `std::find_if_not`

> Spec: <https://en.cppreference.com/cpp/algorithm/find>

## What you learn

The `<algorithm>` find trio:

```cpp
find       (first, last, value);   /* first element == value */
find_if    (first, last, pred);    /* first element pred(x) is TRUE  */
find_if_not(first, last, pred);    /* first element pred(x) is FALSE */
```

`find_if_not` is the C++11 addition. Before C++11 you wrote
`find_if(first, last, std::not1(pred))` -- which only works on
predicates that are *Adaptable* (defined `argument_type`), a
clunky bit of C++03 typing machinery that lambdas made obsolete.
C++11 paired the new lambda syntax with this simpler algorithm
spelling.

All three return:
- An iterator to the first matching element, or
- `last` if no element matches (i.e. the past-the-end iterator).

The canonical idiom for "did we find it?":

```cpp
auto it = std::find_if_not(s.begin(), s.end(), is_whitespace);
if (it == s.end()) { /* string is all whitespace or empty */ }
else { /* it points to the first non-whitespace char */ }
```

### Why this matters for strings

The textbook use is **leading-whitespace trim**:

```cpp
std::string trim_front(const std::string &s)
{
    auto it = std::find_if_not(s.begin(), s.end(),
        [](unsigned char c) { return std::isspace(c); });
    return std::string(it, s.end());
}
```

`find_if_not` walks the string until it sees a non-whitespace
character, then everything from there is the trimmed result.

### The `std::isspace` trap

`std::isspace(int ch)` (in `<cctype>`) has an undefined-behaviour
clause: `ch` must be representable as `unsigned char` or be
`EOF`. A plain `char` from a `std::string` might be `signed
char` on some platforms; values like `0xc3` (the first byte of a
UTF-8 multi-byte) become *negative* `int`s when promoted, which
is outside `unsigned char` range — UB.

The fix is to cast `char` to `unsigned char` before passing it
to `std::isspace`:

```cpp
[](unsigned char c) { return std::isspace(c); }
```

Or:

```cpp
[](char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; }
```

Both are correct. The first form is shorter; the implicit
parameter-type conversion in `find_if_not` does the cast on
each element.

### When to use `find_if` instead

If your predicate is the "true on match" form, `find_if`. If
your predicate is the "true on what you want to skip", you can
either negate it or use `find_if_not`. `find_if_not(pred)` and
`find_if(!pred)` produce the same result; pick whichever reads
more naturally for the predicate name. `find_if_not(is_space)`
is clearer than `find_if(is_not_space)` -- save yourself the
inverted helper.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task81
{
    /* Return the index of the first non-whitespace character in s,
     * or s.size() if s is empty or all whitespace. */
    std::size_t first_non_whitespace(const std::string &s);

    /* Return s with leading whitespace removed. */
    std::string trim_front(const std::string &s);
}
```

Whitespace is "anything `std::isspace` returns true for", which
covers `' '`, `'\t'`, `'\n'`, `'\r'`, `'\v'`, `'\f'`.

## Run

```sh
./verify.sh
```

## Hints

- Both functions wrap a single `std::find_if_not` call. The
  predicate is the `unsigned char` lambda above.

- For `first_non_whitespace`, convert the iterator to an index:
  `static_cast<std::size_t>(it - s.begin())`. If `it == s.end()`,
  the cast gives `s.size()` -- which is the "not found" sentinel
  the spec asks for.

- For `trim_front`, construct the result string from the found
  iterator to `s.end()`: `std::string(it, s.end())`.

- Don't write a hand-rolled loop. The whole point of section O
  is to use the C++11 algorithms.

- `std::isspace` from `<cctype>` matches the C locale's notion of
  whitespace. For locale-aware whitespace you'd reach for the
  C++ locale facets in `<locale>`. For ASCII source code, the
  `<cctype>` version is what you want.

- Common follow-up: `trim_back(s)` does the same with reverse
  iterators. It's a one-liner using `std::find_if_not(s.rbegin(),
  s.rend(), is_space)` plus a forward-iterator conversion via
  `.base()`. Not part of this task, but a fine exercise.
