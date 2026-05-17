# 21 — `_t` alias forms of type traits

> Spec: <https://en.cppreference.com/cpp/header/type_traits>

## What you learn

C++14 added `_t` suffix aliases for every type trait that
had a `::type` member. The aliases collapse the two-step
incantation:

```cpp
typename std::remove_cv<T>::type
```

into the one-step form:

```cpp
std::remove_cv_t<T>
```

Same result, less noise. Especially valuable inside nested
template expressions where `typename` and `::type` pile up.

### Before vs after

C++11 form:

```cpp
template <class T>
typename std::remove_cv<
    typename std::remove_reference<T>::type
>::type
decay_simple(T &&x) {
    return std::forward<T>(x);
}
```

C++14 form:

```cpp
template <class T>
std::remove_cv_t<std::remove_reference_t<T>>
decay_simple(T &&x) {
    return std::forward<T>(x);
}
```

Same code, no `typename` keyword, no `::type` suffix. The
template-heavy parts read more clearly.

### Why `typename` was needed in C++11

`std::remove_cv<T>` is a class template; `::type` is a
dependent name (depends on T). C++ requires `typename`
before any dependent name that's a type:

```cpp
typename std::remove_cv<T>::type    /* C++11 */
```

Without `typename`, the compiler doesn't know whether
`::type` is a type or a static member -- the grammar is
ambiguous. The `_t` alias bakes the `typename` and `::type`
into the alias declaration itself, hiding the ambiguity:

```cpp
template <class T>
using remove_cv_t = typename remove_cv<T>::type;   /* the alias */
```

Now `std::remove_cv_t<T>` is unambiguously a type (it's an
alias-template instantiation, which is always a type).

### Full coverage

C++14 added `_t` aliases for every trait that has a
`::type`:

| Trait                        | Alias                       |
|------------------------------|-----------------------------|
| `remove_cv<T>`               | `remove_cv_t<T>`            |
| `remove_const<T>`            | `remove_const_t<T>`         |
| `remove_volatile<T>`         | `remove_volatile_t<T>`      |
| `add_cv<T>`                  | `add_cv_t<T>`               |
| `add_const<T>`               | `add_const_t<T>`            |
| `add_volatile<T>`            | `add_volatile_t<T>`         |
| `remove_reference<T>`        | `remove_reference_t<T>`     |
| `add_lvalue_reference<T>`    | `add_lvalue_reference_t<T>` |
| `add_rvalue_reference<T>`    | `add_rvalue_reference_t<T>` |
| `remove_pointer<T>`          | `remove_pointer_t<T>`       |
| `add_pointer<T>`             | `add_pointer_t<T>`          |
| `make_signed<T>`             | `make_signed_t<T>`          |
| `make_unsigned<T>`           | `make_unsigned_t<T>`        |
| `remove_extent<T>`           | `remove_extent_t<T>`        |
| `remove_all_extents<T>`      | `remove_all_extents_t<T>`   |
| `aligned_storage<N, A>`      | `aligned_storage_t<N, A>`   |
| `aligned_union<N, Ts...>`    | `aligned_union_t<N, Ts...>` |
| `decay<T>`                   | `decay_t<T>`                |
| `enable_if<B, T>`            | `enable_if_t<B, T>`         |
| `conditional<B, T, U>`       | `conditional_t<B, T, U>`    |
| `common_type<Ts...>`         | `common_type_t<Ts...>`      |
| `underlying_type<T>`         | `underlying_type_t<T>`      |
| `result_of<F(Args...)>`      | `result_of_t<F(Args...)>`   |
| `invoke_result<F, Args...>`  | (C++17 only)                |

The pattern is mechanical: drop `typename` and `::type`,
append `_t` to the trait name.

### The `_v` variable templates (C++17 follow-up)

C++14 added `_t` for traits with `::type`. C++17 added `_v`
for traits with `::value` (boolean predicates):

```cpp
/* C++11 form */
std::is_integral<T>::value

/* C++17 form (variable template) */
std::is_integral_v<T>
```

For C++14 only, you write `::value` for the predicates and
`_t` for the type extractors. Step 7 covered variable
templates, the mechanism behind `_v`. C++14 has the
mechanism but the standard library didn't apply it to traits
until C++17 -- you can roll your own:

```cpp
template <class T>
constexpr bool is_integral_v = std::is_integral<T>::value;
```

### When `_t` matters most

Inside nested templates:

```cpp
/* C++11 -- typename pile-up */
template <class T>
using basic_alias =
    typename std::add_pointer<
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type
    >::type;

/* C++14 -- clean */
template <class T>
using basic_alias =
    std::add_pointer_t<
        std::remove_cv_t<
            std::remove_reference_t<T>>>;
```

The second form is parseable at a glance. The first is a
typename/template parse exercise.

### SFINAE with `enable_if_t`

The `enable_if` SFINAE pattern is much cleaner with the
alias:

```cpp
/* C++11 */
template <class T,
          typename = typename std::enable_if<std::is_integral<T>::value>::type>
void f(T x);

/* C++14 */
template <class T,
          typename = std::enable_if_t<std::is_integral<T>::value>>
void f(T x);
```

Combined with `_v` traits (C++17), you'd write
`enable_if_t<is_integral_v<T>>` -- but for C++14, that's the
template.

### Aliases are not new types

The `_t` aliases instantiate the SAME type as the
`::type` form. They're alias templates, not new types:

```cpp
std::is_same<
    std::remove_cv_t<const int>,
    typename std::remove_cv<const int>::type
>::value   /* true */
```

Use whichever form reads better. In production C++14+ code,
prefer `_t` everywhere; in C++11 codebases, you're stuck
with the verbose form.

## Task

In `solution.hpp`, replace three `typename ...::type` lines
with their C++14 `_t` alias equivalents:

1. `bare_type<T>` should use
   `std::remove_cv_t<std::remove_reference_t<T>>`. The stub
   uses `remove_pointer` (the wrong trait entirely).

2. `pick<Cond, T, U>` should use
   `std::conditional_t<Cond, T, U>`.

3. `only_if<Cond, T>` should use `std::enable_if_t<Cond, T>`.

```cpp
template <class T>
using bare_type = std::remove_cv_t<std::remove_reference_t<T>>;

template <bool Cond, class T, class U>
using pick = std::conditional_t<Cond, T, U>;

template <bool Cond, class T = void>
using only_if = std::enable_if_t<Cond, T>;
```

## Run

```sh
./verify.sh
```

## Hints

- The `_t` aliases are in `<type_traits>`. Already included.

- The mechanical translation: drop `typename`, drop
  `::type`, append `_t` to the trait name.

- `bare_type<T>` strips REF and CV qualifiers, not pointers.
  The stub uses `remove_pointer` which is the wrong trait
  -- `remove_pointer<int*>::type == int`, but
  `bare_type<int*>` should be `int*` (the pointer is part
  of the "bare" type).

- `pick` and `only_if` use traits whose results are types --
  conditional<...>::type for pick, enable_if<...>::type for
  only_if. The aliases just drop the `typename` / `::type`.

- The is_bare_v test catches the bare_type bug specifically:
  `is_bare_v<int*>` should be true (int* IS already
  bare), but the stub's broken bare_type<int*> gives int,
  so is_same<int*, int> is false, so is_bare_v is false ->
  test fails.

- Variable templates (C++14) let us write `_v` aliases for
  boolean traits. The standard library didn't apply this
  until C++17, so for C++14 you write `::value` for
  predicates. The `is_bare_v` in the scaffold is a
  user-defined variable template that wraps `::value`.

- Tests verify: bare_type strips ref/const/volatile/cref/rref;
  bare_type leaves plain int and int* alone; pick picks T
  for true and U for false; only_if compiles only when Cond
  is true; is_bare_v classifies correctly.

- For SFINAE, `std::enable_if_t<Cond, T>` is the most
  common C++14 form: short, no typename, drop-in for the
  C++11 `typename std::enable_if<...>::type`.

- C++17 `_v` traits, when added, make SFINAE one-liners:
  `enable_if_t<is_integral_v<T>>`. For C++14 you write
  `enable_if_t<is_integral<T>::value>`.
