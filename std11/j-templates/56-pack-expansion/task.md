# 56 — Pack expansion

> Spec: <https://en.cppreference.com/cpp/language/parameter_pack>

## What you learn

Step 55 introduced parameter packs and the trick for iterating over
them with side effects. This step focuses on **pack expansion as
data flow** — expanding a pack into a brace-init list, into a
function call, into a base-class list. The pattern is always the
same: a *pattern* followed by `...` is expanded once per element,
with the comma separator the surrounding context expects.

### Common expansion contexts

```cpp
/* Function arguments. */
template <class... Ts>
void invoke(F f, Ts... args) { f(args...); }     /* f(arg0, arg1, ...) */

/* Brace-init list. */
std::vector<int> v{ args... };                    /* v{arg0, arg1, ...} */

/* Function-call expression. */
std::tuple<Ts...> t(args...);                     /* T0, T1, ... and a0, a1, ... */

/* Base classes. */
template <class... Ts> struct combined : Ts... {};

/* sizeof... -- count operator (next step). */
constexpr std::size_t n = sizeof...(Ts);
```

The hint to know: the pattern can be *any* expression that
references the pack. `(value < args ? value : args)...` works,
`static_cast<T>(args)...` works, `f(args, ctx)...` works.

This step's task uses **pack expansion into a brace-init list**
that feeds a standard-library function:

```cpp
template <class T, class... Rest>
T min_of(T first, Rest... rest)
{
    return std::min<T>({first, rest...});
    /*                  ^^^^^^^^^^^^^^^^ pack expansion in brace-init list */
}
```

`std::min` has an overload taking `std::initializer_list<T>`
(C++11). The brace expression `{first, rest...}` constructs that
list with all the arguments. Pack expansion does the heavy lifting.

## Task

In `solution.hpp`, implement:

```cpp
template <class T, class... Rest>
T min_of(T first, Rest... rest);

template <class T, class... Rest>
T max_of(T first, Rest... rest);
```

Each takes at least one argument (the `first` param) and an
optional tail pack. Both must be implemented using pack expansion
into a brace-init list passed to `std::min` / `std::max`.

| Call                                | Returns |
|-------------------------------------|---------|
| `min_of(3)`                         | `3`     |
| `min_of(3, 1)`                      | `1`     |
| `min_of(5, 3, 1, 4, 2)`             | `1`     |
| `min_of(-3, -1, -5, -2)`            | `-5`    |
| `max_of(3)`                         | `3`     |
| `max_of(5, 3, 1, 4, 2)`             | `5`     |
| `max_of(-3, -1, -5, -2)`            | `-1`    |

## Run

```sh
./verify.sh
```

## Hints

- Implementations are one line each:
  ```cpp
  template <class T, class... Rest>
  T min_of(T first, Rest... rest)
  {
      return std::min<T>({first, rest...});
  }
  ```
  Note the explicit `<T>` -- without it, `std::min` would try to
  deduce its template argument from the brace list, which it can't
  do.

- `{first, rest...}` is the pack-expansion pattern. The pattern
  here is just `rest` (each element of the pack); the entire
  brace-list expands to `{first, rest0, rest1, ...}`.

- The `Rest` pack can be empty — `min_of(3)` expands to
  `std::min<T>({3})`, which has the right behavior (an
  initializer_list with one element).

- `<algorithm>` provides `std::min`/`std::max`'s
  `initializer_list` overloads. `<initializer_list>` is pulled in
  transitively but explicit is fine.

- A common mistake: forgetting the `<T>`. The error message reads
  something like "no matching function for call to 'min<...>'"
  with deduction-failure notes.
