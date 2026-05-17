# 100 — `std::make_tuple` and `std::forward_as_tuple`

> Spec: <https://en.cppreference.com/cpp/utility/tuple/make_tuple>,
> <https://en.cppreference.com/cpp/utility/tuple/forward_as_tuple>

## What you learn

C++11 added three "make a tuple from these arguments" helpers,
each with a different argument-handling rule:

| Helper                    | What it stores                              | Use case                                  |
|---------------------------|----------------------------------------------|-------------------------------------------|
| `std::make_tuple(args...)` | Decayed copies / converted types             | Build a value tuple                       |
| `std::forward_as_tuple(args...)` | A tuple of *forwarding references* (`T &` or `T &&`) | Forward arguments into something          |
| `std::tie(args...)`        | A tuple of *lvalue references*               | Capture lvalues for assignment (step 99)  |

### `make_tuple`

The everyday "I have these values, build a tuple" function:

```cpp
auto t = std::make_tuple(1, 3.14, std::string("hi"));
/* t : std::tuple<int, double, std::string> */
```

Important deduction quirk: `make_tuple("hi")` deduces
`tuple<const char *>`, not `tuple<std::string>`. If you want a
string, pass `std::string("hi")` or wrap in `std::string{...}`
explicitly.

Reference-wrappers are special-cased: `make_tuple(std::ref(x))`
gives `tuple<int &>` (preserves the wrapper's intent).

### `forward_as_tuple`

The "package up arguments without copying" function. It's what
you use to feed arguments into an in-place construction or
piecewise mechanism:

```cpp
auto t = std::forward_as_tuple(1, std::string("hi"), some_lvalue);
/* t : std::tuple<int &&, std::string &&, SomeType &> */
```

The element types are *forwarding references*: lvalues come
out as `T &`, rvalues as `T &&`. The tuple itself doesn't own
the values -- it references them. So **the result is only safe
to use while the arguments are alive**. Typical use: pass the
result immediately into another function in the same
expression; don't store it in a variable that outlives the
arguments.

### `std::piecewise_construct` and `std::pair`

The canonical use of `forward_as_tuple` is piecewise pair
construction:

```cpp
std::pair<A, B> p(std::piecewise_construct,
                  std::forward_as_tuple(/* A ctor args */),
                  std::forward_as_tuple(/* B ctor args */));
```

`std::piecewise_construct` is a tag that selects an overload of
`std::pair`'s constructor which:

1. Unpacks the first `forward_as_tuple` and uses its elements to
   *construct A in place*.
2. Same for B.

Without this mechanism, you'd have to build temporary A and B
objects first, then move them into the pair:

```cpp
std::pair<A, B> p(A(/* A args */), B(/* B args */));
/* two extra moves / one extra copy each, depending on types */
```

Piecewise construction avoids those temporaries. It's the same
optimisation `emplace_back` does for a single-element container
push, generalised to pairs (and, by extension, to map's
`emplace(piecewise_construct, ...)` patterns).

### When does this matter?

The classic case is `std::map<K, V>::emplace` for a map whose
value type isn't trivially constructible. Without
piecewise-construct, `emplace(K(...), V(...))` would build
temporaries; with it, both K and V are constructed in place in
the node.

For everyday code, `std::make_tuple` is what you use 90% of the
time. `forward_as_tuple` shows up when implementing generic
code that needs to forward arguments through tuples (like a
custom `apply` or pre-C++17 `invoke`).

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task100
{
    /* A type with a deliberate non-trivial constructor. */
    struct Point
    {
        int x, y;
        Point(int xx, int yy) : x(xx), y(yy) {}
    };

    /* A type with a different non-trivial constructor. */
    struct Label
    {
        std::string text;
        double weight;
        Label(std::string t, double w) : text(std::move(t)), weight(w) {}
    };

    /* Build a tuple<int, double, std::string> via std::make_tuple. */
    std::tuple<int, double, std::string> via_make_tuple(int i, double d, std::string s);

    /* Build a std::pair<Point, Label> using piecewise_construct +
     * two forward_as_tuple()s, so both Point and Label are
     * constructed in place. */
    std::pair<Point, Label>
    via_piecewise(int px, int py, std::string ltext, double lweight);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `via_make_tuple`:
  ```cpp
  return std::make_tuple(i, d, std::move(s));
  ```
  Pass the string by value through `std::move` to avoid one
  copy when it lands inside the tuple.

- `via_piecewise`:
  ```cpp
  return std::pair<Point, Label>(
      std::piecewise_construct,
      std::forward_as_tuple(px, py),
      std::forward_as_tuple(std::move(ltext), lweight));
  ```
  The first `forward_as_tuple` packages Point's constructor
  args; the second packages Label's. Both get unpacked into
  in-place constructor calls.

- Don't forget `std::move(ltext)`. The Label constructor takes
  the string by value (and moves into the member), so forwarding
  the rvalue avoids a copy.

- `std::piecewise_construct` is a value, not a type --
  `std::piecewise_construct_t` is its type. Pass the value as
  the first argument.

- `std::pair`'s `(piecewise_construct, ttuple1, ttuple2)`
  constructor doesn't care that the inner tuples were built
  with `forward_as_tuple` specifically. You could pass plain
  `make_tuple(...)` -- but then the tuple's element types
  would be the *decayed* types and additional moves would
  happen. `forward_as_tuple` is the zero-copy variant.

- Common confusion: "why not just `pair<Point, Label>(Point(px,
  py), Label(text, w))`?" -- because that builds named
  temporaries and moves them in. Piecewise avoids the
  temporaries entirely. For cheap types it doesn't matter; for
  types with large state it does.

- C++17 introduces class template argument deduction (CTAD)
  which lets you write `std::pair(...)` without spelling the
  type, and the piecewise_construct constructor is largely
  obsoleted by `std::map<K, V>::try_emplace` and similar.
  C++11/14 codebases still use the piecewise dance often.
