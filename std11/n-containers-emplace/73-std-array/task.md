# 73 — `std::array<T, N>`

> Spec: <https://en.cppreference.com/cpp/container/array>

## What you learn

`std::array<T, N>` is the smallest possible container in the
standard library: a fixed-size sequence of `T` whose layout is
exactly that of a built-in array `T[N]`. The size `N` is a
template parameter, not a runtime value — it's baked into the
type.

What it gives you over a raw C array:

- **Container interface**: `size()`, `empty()`, `front()`,
  `back()`, `data()`, `begin()`/`end()`, `at()` (bounds-checked).
- **Value semantics**: copy/move/swap all work like any other
  value type. `arr1 = arr2;` copies element-by-element. Raw
  arrays decay into pointers on assignment — that's the trap
  `std::array` was made to remove.
- **Comparison**: `==`, `!=`, `<`, etc. compare element-by-
  element. Raw arrays compare *pointers*.
- **No decay**: passing a `std::array` to a function actually
  passes the array (by value or reference), not a pointer.

What it doesn't give you:

- It is not resizable. The whole point is that the size is part
  of the type.
- It is not dynamically allocated. Storage is wherever the
  `std::array` itself lives — usually on the stack, sometimes in
  static storage, sometimes inside another object. **It never
  heap-allocates.**
- `sizeof(std::array<T, N>) == N * sizeof(T)`. No header, no
  padding for a size field, no virtual table. The implementation
  is essentially `struct { T data[N]; }` plus member functions.

### Aggregate initialisation

`std::array` is an aggregate. You initialise it with **brace-
init**:

```cpp
std::array<int, 4> a{1, 2, 3, 4};
std::array<int, 4> z{};            /* all zeros */
```

In C++11 you may sometimes need the older double-brace form
(`std::array<int, 4> a{{1, 2, 3, 4}};`) because of how aggregate
initialisation interacts with the wrapped C array. Most C++11
compilers accept the single-brace form; the standard officially
made it always-allowed in C++14.

### Default initialisation

```cpp
std::array<int, 4> a;          /* indeterminate values for trivial T */
std::array<int, 4> b{};        /* all value-initialised -> zero */
```

This matches built-in arrays. If `T` is a class type with a
non-trivial default constructor, the first form runs the
constructor on each element; for trivially-default-constructible
types you get garbage. Use `{}` if you want zeroed elements.

### Range-for / iterators

```cpp
double total = 0;
for (double x : a) total += x;
```

You can also walk by iterator:

```cpp
for (auto it = a.begin(); it != a.end(); ++it) total += *it;
```

Or use a standard algorithm:

```cpp
double total = std::accumulate(a.begin(), a.end(), 0.0);
```

(`std::accumulate` is in `<numeric>`.)

## Task

In `solution.hpp`, implement three functions over
`std::array<double, 4>`:

```cpp
namespace task73
{
    double sum(const std::array<double, 4> &a);
    double mean(const std::array<double, 4> &a);

    /* Component-wise addition: returns {x[0]+y[0], ...}. */
    std::array<double, 4> add(const std::array<double, 4> &x,
                              const std::array<double, 4> &y);
}
```

`mean` is `sum / 4.0`. `add` returns a fresh `std::array` whose
elements are the pairwise sums.

## Run

```sh
./verify.sh
```

## Hints

- All three functions are short. Use range-for for `sum` and a
  loop or aggregate-init for `add`.

- For `add`, you can either write:
  `std::array<double, 4> r{}; for (...) r[i] = x[i] + y[i]; return r;`
  or use aggregate initialisation:
  `return {x[0] + y[0], x[1] + y[1], x[2] + y[2], x[3] + y[3]};`.

- The `N` template parameter means `add` for `std::array<T, 4>`
  is a different function from `add` for `std::array<T, 5>`. To
  write one function for any size, you'd use a function template
  — but that's not what this step is about.

- `at(i)` is the bounds-checked accessor (throws on out-of-range).
  `operator[](i)` is unchecked. Use the unchecked form here since
  the index is always in range.

- `size()` is a `constexpr` member function on `std::array`. You
  could do `mean(a) = sum(a) / a.size();` and the compiler can
  fold `a.size()` to 4. (We hard-code the 4.0 here so the maths
  is obvious; in real code, prefer `a.size()` so it stays right
  if you change `N`.)

- `std::array` was specifically added to fix the long list of
  papercuts with raw C arrays: decay to pointer on argument
  passing, no comparison operators, no `.size()`, no swap, etc.
  Modern C++ code should reach for `std::array` first and only
  fall back to `T[N]` for low-level / C-interop work.
