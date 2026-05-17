# 105 — `std::common_type`

> Spec: <https://en.cppreference.com/cpp/types/common_type>

## What you learn

`std::common_type<Args...>::type` is **the type to which all
arguments can be implicitly converted**. It's the metafunction
behind the usual-arithmetic-conversion rules — the same rules
that decide the result type of `1 + 2.5` (double) or
`'a' + 1u` (unsigned).

The general definition:

```cpp
std::common_type<int, double>::type             /* double */
std::common_type<int, long, double>::type        /* double */
std::common_type<int, unsigned>::type            /* unsigned (with caveats) */
std::common_type<int, int, int>::type            /* int */
```

For most uses it Just Works: integral promotions promote, mixed
signedness goes to unsigned, mixed integer-and-float goes to
the floating-point type, and so on.

### Where it shows up

Anywhere you write a generic function that takes "several
numeric inputs of possibly different types" and you need to
*declare* a single result type:

```cpp
template <class A, class B>
typename std::common_type<A, B>::type avg(A a, B b)
{
    using R = typename std::common_type<A, B>::type;
    return (static_cast<R>(a) + static_cast<R>(b)) / R(2);
}

avg(1, 2.5);   /* return type: double, result: 1.75 */
```

Without `common_type`, you'd have to manually pick one (`A`?
`B`? promote to `double` always?) and probably get the corners
wrong.

### Variadic `max`

The variadic generalisation is the headline use:

```cpp
template <class T>
T max(T x) { return x; }                          /* base case */

template <class A, class B>
typename std::common_type<A, B>::type max(A a, B b)
{
    using R = typename std::common_type<A, B>::type;
    return a < b ? static_cast<R>(b) : static_cast<R>(a);
}

template <class A, class B, class... Rest>
typename std::common_type<A, B, Rest...>::type max(A a, B b, Rest... rest)
{
    return max(max(a, b), rest...);
}
```

Three overloads:
- One argument: return as-is.
- Two arguments: standard binary max with the common-type
  promotion.
- More: pairwise reduce — `max(a, b)` first, then recurse with
  the result and the rest.

The recursion is the standard variadic-template pattern (steps
55-58 covered packs). `common_type<A, B, Rest...>` handles
arbitrary-arity result-type promotion at the top level; the
recursion makes sure the runtime computation goes through.

### Conversion-rank quirks worth noting

```cpp
std::common_type<int, unsigned>::type     /* unsigned int */
std::common_type<signed char, char>::type  /* int (integral promotion) */
std::common_type<float, int>::type         /* float */
std::common_type<std::string, const char *>::type /* std::string */
```

These follow the language's implicit-conversion rules. If two
types have no common type (e.g. `common_type<int, std::string>`
when neither converts to the other), the trait yields a class
with no `::type`. That's a hard error at template
instantiation, which is the intended diagnostic.

### `common_type_t` (C++14)

```cpp
std::common_type_t<int, double>          /* C++14 shortcut */
```

C++11 makes you write `typename std::common_type<int, double>::type`.
For this task, the C++11 form is what you use.

## Task

In `solution.hpp`, implement a variadic `max`:

```cpp
namespace task105
{
    /* Single-arg, two-arg, and variadic max. The return type is the
     * std::common_type of all arguments. */

    template <class T>
    T max(T x);

    template <class A, class B>
    typename std::common_type<A, B>::type max(A a, B b);

    template <class A, class B, class... Rest>
    typename std::common_type<A, B, Rest...>::type max(A a, B b, Rest... rest);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The three-overload structure is the simplest:
  ```cpp
  template <class T>
  T max(T x) { return x; }

  template <class A, class B>
  typename std::common_type<A, B>::type max(A a, B b)
  {
      using R = typename std::common_type<A, B>::type;
      return a < b ? static_cast<R>(b) : static_cast<R>(a);
  }

  template <class A, class B, class... Rest>
  typename std::common_type<A, B, Rest...>::type
  max(A a, B b, Rest... rest)
  {
      return max(max(a, b), rest...);
  }
  ```

- The `static_cast<R>` in the two-arg form avoids
  signed/unsigned warning noise on some compilers and makes the
  return-type contract explicit.

- The variadic recursion reduces by one each step:
  `max(a, b, c, d)` -> `max(max(a, b), c, d)` -> ...
  -> `max(max(max(a, b), c), d)`. Each step has two-or-more
  arguments until it's exactly two, at which point the
  two-argument overload terminates.

- Don't try to use a fold expression. Folds are C++17. C++11
  uses recursion + pack expansion.

- The single-argument `max(T)` overload is needed: without it,
  calling `max(42)` with a single argument would have no
  viable overload. The cases are 1, 2, or 3+ args; the
  variadic overload requires *at least* 2, so the 1-arg
  overload is the unambiguous base.

- The tests use `decltype(task105::max(args...))` in
  `static_assert`s to pin the return type alongside the
  runtime value checks.

- `common_type` handles the *usual-arithmetic-conversion*
  rules: mixed int/double -> double, mixed signed/unsigned ->
  unsigned, integral promotions to int. The trait *is* those
  rules expressed at the type level.
