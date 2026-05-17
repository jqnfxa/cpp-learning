# 92 — free `std::begin` and `std::end`

> Spec: <https://en.cppreference.com/cpp/iterator/begin>,
> <https://en.cppreference.com/cpp/iterator/end>

## What you learn

C++11 added the free functions `std::begin(c)` and `std::end(c)`
in `<iterator>`. They give you a *uniform* way to get
iterators into any range — whether it's a standard container, a
C array, or your own type that provides member `begin()` and
`end()`.

```cpp
std::vector<int> v = {1, 2, 3};
int             arr[] = {1, 2, 3};

auto it1 = std::begin(v);     /* calls v.begin() */
auto it2 = std::begin(arr);   /* returns &arr[0] -- T * */
```

The free `std::begin` is implemented (roughly) as:

```cpp
template <class C>
auto begin(C &c) -> decltype(c.begin()) { return c.begin(); }

/* And a C-array overload: */
template <class T, std::size_t N>
T *begin(T (&arr)[N]) { return arr; }
```

The C-array overload is the headline. Built-in arrays don't have
member functions, so `arr.begin()` doesn't compile. Free
`std::begin(arr)` does the right thing automatically.

### Why this matters for generic code

Before C++11, "iterate over any range" required overload sets:

```cpp
template <class C> void f(const C &c) { for (auto x : c) ... }   /* ok */

template <class T, std::size_t N>                                 /* needed for arrays */
void f(const T (&arr)[N]) { for (auto x : arr) ... }
```

Now you write one function:

```cpp
template <class C>
std::string to_string_range(const C &c)
{
    using std::begin;
    using std::end;
    for (auto it = begin(c); it != end(c); ++it)
    {
        /* ... */
    }
}
```

And it works for `std::vector`, `std::array`, `std::list`, raw
arrays, your own ranges -- anything that has either
`.begin()`/`.end()` or fits the C-array template specialisation.

### The `using std::begin; ... begin(c);` ADL trick

If you wrote `std::begin(c)` directly, your function would only
look up `begin` in namespace `std`. That misses
argument-dependent lookup (ADL) for user-defined ranges in
their own namespace.

The idiomatic spelling is:

```cpp
using std::begin;     /* bring std::begin into scope */
auto it = begin(c);   /* unqualified call -- ADL kicks in */
```

This means: if `c`'s type lives in some namespace `N`, and `N`
provides its own `begin(c)`, that's the one called. If not,
the `using std::begin` fallback applies. Together they cover
every range type uniformly.

This is the **niebloid-precursor pattern** that ranges (C++20)
later formalises with the `std::ranges::begin` customisation
point object. For C++11 the using-and-unqualified-call dance is
what you write.

### Range-based `for` already does this

The C++11 range-for loop's translation uses the same
`begin/end` lookup rules. So if range-for works, your free
`begin/end` will too. It's the same machinery from a different
angle.

## Task

In `solution.hpp`, write one function template:

```cpp
namespace task92
{
    /* Format the elements of c as "[v0, v1, v2, ..., vN]".
     * Empty range -> "[]". Single element -> "[42]". */
    template <class C>
    std::string to_string(const C &c);
}
```

It must work for:
- `std::vector<int>` (member begin/end)
- `std::array<int, N>` (member begin/end)
- `int[]` (C array — free begin/end)

Use `std::to_string` on each element to build the formatted
output.

## Run

```sh
./verify.sh
```

## Hints

- The body is the using-declaration + unqualified call pattern:
  ```cpp
  template <class C>
  std::string to_string(const C &c)
  {
      using std::begin;
      using std::end;

      std::string out = "[";
      bool first = true;
      for (auto it = begin(c); it != end(c); ++it)
      {
          if (!first) out += ", ";
          first = false;
          out += std::to_string(*it);
      }
      out += "]";
      return out;
  }
  ```

- Templates can't be `inline`-marked the same way as non-template
  functions (well, they can, but they're already implicitly
  inline by being defined in a header). The convention is to
  define function templates in headers without an explicit
  `inline`.

- The C-array test passes an `int[5]` to the function. The
  template parameter `C` deduces to `int[5]`, and free
  `std::begin(c)` returns `int *`. From there, iteration is
  identical to vector iteration.

- Don't try to cast a C array to a pointer or take `c[0]` /
  `&c[0]`. The whole point is that `std::begin` /
  `std::end` make the array case work the same as the container
  case.

- `std::to_string` (from `<string>`) covers int / long / double
  / etc. -- you don't need a custom formatter. The
  `to_string_range` function name in the task is a misnomer
  worth pointing out: don't collide with the global
  `std::to_string` -- we keep it inside `task92::` namespace.

- The same pattern generalises to `cbegin`/`cend` (C++14),
  `rbegin`/`rend` (C++14), and `size` (C++17). The C++11
  version covers `begin`/`end` only.
