# 102 — transformation traits: `remove_reference`, `remove_cv`, `decay`

> Spec: <https://en.cppreference.com/cpp/header/type_traits>

## What you learn

Where the property traits (step 101) return a *value*
(`::value`), transformation traits return a *type* (`::type`).
They're the "remove this attribute / decay to that form" tools
of `<type_traits>`.

The three most useful ones in everyday template code:

| Trait                          | Effect                                          |
|--------------------------------|-------------------------------------------------|
| `std::remove_reference<T>::type` | `T &` and `T &&` -> `T`                          |
| `std::remove_cv<T>::type`        | `const T`, `volatile T`, `const volatile T` -> `T` |
| `std::decay<T>::type`            | Full "by-value parameter" decay (see below)     |

### `remove_reference`

The simplest transformation:

```cpp
remove_reference<int>::type     == int     /* unchanged */
remove_reference<int &>::type   == int
remove_reference<int &&>::type  == int
remove_reference<const int &>::type == const int  /* CV stays */
```

It only touches the reference qualifier. CV qualifications on
the underlying type survive.

### `remove_cv`

Strips *top-level* `const` and/or `volatile`:

```cpp
remove_cv<const int>::type           == int
remove_cv<volatile int>::type        == int
remove_cv<const volatile int>::type  == int
remove_cv<int>::type                 == int       /* unchanged */
remove_cv<const int *>::type         == const int *  /* the const is on the
                                                       pointee, not top-level */
remove_cv<int *const>::type          == int *     /* top-level const -> stripped */
```

The "top-level" qualifier matters: `int *const` has the const on
the pointer itself (`remove_cv` strips it); `const int *` has
the const on what it points at (`remove_cv` doesn't touch it).

### `decay`

The "full pass-by-value parameter" transformation. It does the
same conversions the compiler does when you take a parameter by
value in a function. Equivalent to:

1. `remove_reference<T>` first.
2. If the result is an array type, decay it to a pointer.
3. If the result is a function type, decay it to a function
   pointer.
4. Otherwise, `remove_cv` of the result.

```cpp
decay<int &>::type            == int
decay<const int &>::type       == int
decay<int[5]>::type            == int *      /* array-to-pointer */
decay<int(int)>::type          == int(*)(int) /* function-to-pointer */
decay<const std::string &>::type == std::string
```

`decay` is what you want when you're capturing "what would a
by-value parameter look like?" in a metafunction. It's how
`std::function`, `std::tuple`'s constructor, and forwarding
machinery deduce the "stored" type from an argument expression.

### The combined "bare type"

For perfect-forwarding generic code, you often want "strip
references and top-level cv, keep arrays as arrays and
functions as functions". That's:

```cpp
template <class T>
struct bare_type
{
    using type = typename std::remove_cv<
        typename std::remove_reference<T>::type
    >::type;
};
```

This is what step 102 asks you to build. It's like `decay`
but without the array-to-pointer and function-to-pointer
conversions. Useful when you want to know the "underlying T"
of a forwarding reference parameter without losing array-ness
or function-ness.

### Using transformation traits

The two main spellings:

```cpp
typename std::remove_reference<T>::type      /* the C++11 form */
std::remove_reference_t<T>                    /* the C++14 alias */
```

In C++11 you write the verbose form. The `typename` keyword is
required because `::type` is a dependent name.

## Task

In `solution.hpp`, define one metafunction:

```cpp
namespace task102
{
    /* Strip references and top-level cv from T. */
    template <class T>
    struct bare_type
    {
        using type = ...;
    };
}
```

`bare_type<T>::type` must:
- Yield `T` for plain types.
- Strip `&` and `&&` references.
- Strip top-level `const` / `volatile`.
- Leave arrays and function types alone (unlike `decay`).

Validation is via `static_assert` -- no runtime tests beyond a
smoke test.

## Run

```sh
./verify.sh
```

## Hints

- The body is a compose of two existing traits:
  ```cpp
  using type = typename std::remove_cv<
      typename std::remove_reference<T>::type
  >::type;
  ```

  Read inside out: first strip references, then strip top-level
  cv from the result. The two `typename` keywords are required
  because `::type` is a dependent name in template context.

- Don't use `std::decay`. It does more than the task asks
  (arrays decay to pointers, functions decay to function
  pointers). The task wants just "strip refs and cv".

- The order matters: `remove_reference` first, *then*
  `remove_cv`. Doing `remove_cv` first on `const int &` would
  see the reference (which is itself never const) and do
  nothing, leaving you with `const int &` to feed to
  `remove_reference`. Correct order strips `const int &` ->
  `const int` -> `int`.

- The `static_assert`s in `tests.cpp` use
  `std::is_same<A, B>::value`. Both sides are computed at
  compile time; failures show up as build errors.

- The smoke `TEST(...)` exists so ctest has a runtime artifact
  to run; the actual verification is the build-time
  static_asserts.

- C++14 adds the `_t` aliases (`remove_reference_t`,
  `remove_cv_t`, `decay_t`) that drop the `typename`-and-
  `::type` ceremony. You'll see them everywhere in modern
  code. The C++11 spelling is the same operations, more verbose.

- Knowing the difference between `decay` and `bare_type`
  matters when forwarding: `decay` is for "what would by-value
  do?" and `bare_type` is for "what's the underlying type if I
  ignore qualifiers?". Different questions, same plumbing.
