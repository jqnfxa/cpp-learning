# 104 — `std::conditional`

> Spec: <https://en.cppreference.com/cpp/types/conditional>

## What you learn

`std::conditional<B, T, F>::type` is the metafunction analogue
of `B ? T : F` for *types*:

```cpp
std::conditional<true,  int, double>::type   /* == int */
std::conditional<false, int, double>::type   /* == double */
```

It's implemented (about) like this:

```cpp
template <bool B, class T, class F> struct conditional      { using type = T; };
template <class T, class F>         struct conditional<false, T, F> { using type = F; };
```

The primary template picks `T`; the partial specialisation for
`false` picks `F`. The selection happens at compile time, so
both arms must be valid types but they can be very different
shapes -- one int, one std::string, one a void pointer, doesn't
matter.

### Where you use it

The most common pattern is "pick the type that satisfies some
condition I can express as a `bool`-valued trait":

```cpp
template <class A, class B>
struct wider                              /* picks the wider integral type */
{
    using type = typename std::conditional<(sizeof(A) >= sizeof(B)), A, B>::type;
};

wider<int, char>::type      /* == int */
wider<short, long long>::type /* == long long */
```

This is `enable_if` for the case where you want *both* arms to
exist (instead of removing the overload). Both traits live in
`<type_traits>`; they're complementary tools.

### `conditional_t` (C++14)

C++14 adds the `_t` alias that drops the `typename ... ::type`
ceremony:

```cpp
std::conditional_t<B, T, F>   /* same as typename std::conditional<B, T, F>::type */
```

For C++11 you write the full form. (Many of these tasks
deliberately use the C++11 form to keep the boilerplate visible
-- in production C++14+, `_t` aliases are almost always nicer.)

### Lazy vs eager evaluation

`std::conditional` is *eager*: both `T` and `F` must be valid
types, not just the chosen one. That's the limitation -- if you
need a "use this expression that's invalid for the other arm"
behaviour, you'd need a deeper trait-based dispatch
(specialised template).

```cpp
/* This would not compile if T isn't a class with ::value_type: */
typename std::conditional<HasValueType<T>::value,
                          typename T::value_type,
                          int>::type
```

(Even if `HasValueType<T>::value` is false, the compiler still
evaluates `typename T::value_type` while substituting -- and
fails for types without `value_type`. The cure is wrapping in a
trait that defers, like an `identity<T>` shim.)

For most uses -- picking among already-valid types -- this
limitation doesn't matter. The wider-of-two-integers task is in
that "easy" zone.

### The "biggest of N types" generalisation

The pairwise form generalises to variadic via recursion:

```cpp
template <class A, class... Rest>
struct widest
{
    using type = typename wider<A, typename widest<Rest...>::type>::type;
};

template <class A>
struct widest<A> { using type = A; };
```

Not part of this task; mentioned to show that the pairwise
primitive is enough.

## Task

In `solution.hpp`, define one metafunction:

```cpp
namespace task104
{
    /* The wider of two integral types, by sizeof.
     * Tie-breaker: prefer A. */
    template <class A, class B>
    struct wider
    {
        using type = ...;
    };
}
```

Validation is via `static_assert` -- no runtime tests beyond a
smoke.

## Run

```sh
./verify.sh
```

## Hints

- The body is one line:
  ```cpp
  using type = typename std::conditional<
      (sizeof(A) >= sizeof(B)), A, B>::type;
  ```
  The parentheses around `sizeof(A) >= sizeof(B)` aren't
  strictly required, but they make the boolean condition
  unambiguous to a reader.

- The tie-breaker is `>=` (not `>`): when sizeof(A) ==
  sizeof(B), pick A. This makes `wider<int, int>::type == int`
  rather than the `B` arm, which is the convention the tests
  encode.

- Don't try to compare *which is larger* -- `std::conditional`
  doesn't pick a winner, you provide the boolean. The boolean
  here is `sizeof(A) >= sizeof(B)`. For more interesting
  decisions (signed/unsigned, integral/floating-point), you'd
  compose with other traits.

- `wider<int, long long>::type` is `long long` on every common
  platform (int = 4 bytes, long long = 8). Tests rely on this
  ordering.

- C++14's `_t` shortcut would make the metafunction's body
  shorter: `using type = std::conditional_t<COND, A, B>;`. In
  C++11 you write the full `typename ... ::type` form.

- Real uses of `std::conditional`: tag dispatch on
  is_integral, alias-template aliases that change based on a
  size parameter, choosing between two "lying-around" types
  that the compiler should pick at instantiation time.

- The "lazy" limitation matters when you need *one arm to be
  invalid* and the other valid. The `enable_if` / specialisation
  approach handles those; `conditional` is for the easy case
  where both arms are well-formed.
