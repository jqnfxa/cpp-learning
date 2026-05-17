# 62 — `decltype`

> Spec: <https://en.cppreference.com/cpp/language/decltype>

## What you learn

`decltype(EXPR)` yields the *type* of `EXPR` — at compile time,
without evaluating. Unlike `auto`:

- `auto` drops reference and top-level cv qualifiers (template
  deduction rules).
- `decltype` preserves them, and *also* encodes the value category
  of the expression in the result.

Two grammars worth distinguishing:

| Form                 | Result                                      |
|----------------------|---------------------------------------------|
| `decltype(name)`      | the *declared* type of the entity            |
| `decltype((expr))`    | the *expression* type — adds & for lvalues, && for xvalues |

```cpp
int  x = 0;
int &r = x;

decltype(x)   a;       /* int -- declared type of `x` */
decltype(r)   b = x;   /* int & -- declared type of `r` */
decltype((x)) c = x;   /* int & -- expression form, x is an lvalue */
decltype(0)   d;       /* int -- 0 is a prvalue, no reference */
```

The expression form (with parens) is what step 29 used to classify
value categories: `is_lvalue_reference<decltype((expr))>` is true
exactly when `expr` is an lvalue.

### `std::declval` — the "no-evaluation" companion

`std::declval<T>()` is a function template *declared but not
defined* that returns `T &&`. It exists only so you can spell
"a T value" inside `decltype` (an unevaluated context). You can't
actually call it at runtime, but you can use it in `decltype` to
extract types that depend on member functions.

```cpp
/* Get the element type of any container with begin(). */
template <class T>
using element_t = decltype(*std::declval<T>().begin());
```

Step by step:
- `std::declval<T>()` -- "as if I have a T value here"
- `.begin()` -- get the iterator
- `*` -- dereference to the element
- `decltype(...)` -- the type of that dereference (typically `T &`
  or `const T &`)

This is the C++11 idiom for *type traits that depend on member-
function return types*. C++14 alias-template variants make it
nicer; the core idea is C++11.

## Task

In `solution.hpp`, declare three type aliases:

```cpp
/* The declared type of an entity. */
template <class T>
using declared_t = decltype(std::declval<T>());

/* The element type of a container (what *c.begin() yields). */
template <class T>
using element_t = decltype(*std::declval<T>().begin());

/* The type of the result of `a + b`. */
template <class A, class B>
using sum_t = decltype(std::declval<A>() + std::declval<B>());
```

`tests.cpp` uses `std::is_same` to pin the alias results.

| Alias                                                 | Result                |
|-------------------------------------------------------|-----------------------|
| `element_t<std::vector<int>>`                         | `int &`               |
| `element_t<const std::vector<int>>`                   | `const int &`         |
| `element_t<std::vector<std::string>>`                 | `std::string &`       |
| `sum_t<int, int>`                                     | `int`                 |
| `sum_t<int, double>`                                  | `double`              |
| `sum_t<short, char>`                                  | `int` (usual arithmetic conversions promote both to int) |

## Run

```sh
./verify.sh
```

## Hints

- All three aliases are one line each. No bodies.

- For `element_t`, the `*` is what makes it the element rather than
  the iterator. `decltype(std::declval<T>().begin())` would yield
  the iterator type instead.

- The aliases are entirely metaprogramming -- they never run. The
  static_asserts in tests.cpp are the validation.

- A common confusion: `decltype(*v.begin())` (without `declval`)
  doesn't work for type-only contexts where you don't have a `v`.
  `declval<T>()` is what gives you a "value" of `T` in an
  unevaluated context.

- C++20 brings `std::ranges::range_value_t<T>` and similar that
  replace these hand-rolled aliases. For C++11, you write them
  yourself.
