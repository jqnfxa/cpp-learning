# 55 — Variadic templates: parameter packs

> Spec: <https://en.cppreference.com/cpp/language/parameter_pack>

## What you learn

A **parameter pack** is a template parameter (or function
parameter) that holds *zero or more* arguments. Declaration uses
`...`:

```cpp
template <class... Ts>            /* type parameter pack: Ts */
void f(Ts... args);               /* function parameter pack: args */
```

Two flavors of pack:

- **Type parameter pack** (`class... Ts`): a list of *types*.
- **Function parameter pack** (`Ts... args`): a list of *values*,
  each typed by the corresponding element of `Ts`.

You can't index them directly like an array. You operate on them
through **pack expansion** (this step) and **recursion** (step 58).

### Pack expansion

Any pattern followed by `...` is expanded once per element:

```cpp
template <class... Ts>
void f(Ts... args)
{
    /* each pattern expands once per element */
    g(args...);                    /* g(args0, args1, args2, ...) */
    h(static_cast<int>(args)...);  /* h((int)args0, (int)args1, ...) */
}
```

The C++11 idiom for "apply some side-effecting operation to every
element of a pack" is the **initializer-list trick**:

```cpp
template <class... Ts>
std::string concat(const Ts &...args)
{
    std::ostringstream oss;
    using expander = int[];                   /* zero-cost dummy type */
    (void)expander{0, ((void)(oss << args), 0)...};
    return oss.str();
}
```

Read the trick:
- `((void)(oss << args), 0)` is a comma-expression that streams
  one element and yields `0`.
- The `...` expands that whole expression once per pack element,
  separated by commas.
- The outer `(void)expander{0, ...}` builds a throwaway array of
  ints. The array-init order is well-defined (left to right), so
  side effects fire in order.
- The leading `0` makes the trick work for the empty-pack case
  (the resulting array would have zero elements otherwise — UB).

C++17 fold expressions turn this into a one-liner; for C++11 the
init-list trick is the standard pattern.

## Task

In `solution.hpp`, implement two function templates:

```cpp
template <class... Ts>
std::string concat(const Ts &...args);

template <class... Ts>
constexpr std::size_t arity(const Ts &.../* args */);
```

`concat` streams each argument into a `std::ostringstream` (no
separator) and returns the resulting string. `arity` returns the
number of arguments using `sizeof...`.

| Call                                | `concat(...)` | `arity(...)` |
|-------------------------------------|---------------|--------------|
| `()`                                 | `""`          | `0`          |
| `(1)`                                | `"1"`         | `1`          |
| `(1, 2, 3)`                          | `"123"`       | `3`          |
| `("hello ", "world")`                | `"hello world"` | `2`        |
| `(1, "+", 2, "=", 3)`                | `"1+2=3"`     | `5`          |

## Run

```sh
./verify.sh
```

## Hints

- `concat`: copy the initializer-list pattern verbatim. Don't try
  to write a `for (auto x : args)` loop — packs aren't iterable
  that way in C++11.

- `arity`: one line — `return sizeof...(Ts);` (or `sizeof...(args)`,
  same effect). `sizeof...` is the **count** operator for packs;
  step 57 covers it in more depth.

- `arity` is `constexpr` because `sizeof...` is a constant
  expression. Tests put it in `static_assert`.

- Include `<sstream>` (for `std::ostringstream`), `<string>` (for
  `std::string`), and `<cstddef>` (for `std::size_t`).

- A common mistake: forgetting to expand the pack. Writing
  `oss << args` (no `...`) is a type error — `args` is a *pack*,
  not a single value, and you can't pass a pack as if it were one
  argument.
