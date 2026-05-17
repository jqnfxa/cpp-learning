# 7 — Variable templates

> Spec: <https://en.cppreference.com/cpp/language/variable_template>

## What you learn

C++14 added **variable templates**: a template whose entity
is a *variable*, not a function or class. The textbook
motivating use is `pi<T>`:

```cpp
template <class T>
constexpr T pi = T(3.14159265358979323846L);

float        a = pi<float>;        /* 3.1415927f */
double       b = pi<double>;       /* 3.14159265358979 */
long double  c = pi<long double>;  /* full L precision */
```

One declaration, three instantiations -- each with the right
storage type and a value initialised from the long-double
literal narrowed to T.

### Before C++14: function-template workaround

In C++11 you'd write a function template that returns the
value:

```cpp
template <class T>
constexpr T pi() { return T(3.14159265358979L); }

float a = pi<float>();    /* note the () */
```

Annoying for two reasons:
1. The call site needs `()`. Not a constant *expression*
   in the C++11 sense; it's a function call.
2. Many compilers wouldn't inline the call as aggressively
   as a direct variable read.

The variable template form drops the parens and is treated
as a direct constant value.

### Before C++14: class-template + static member

The other workaround was a class template with a static
constexpr member:

```cpp
template <class T>
struct pi_traits {
    static constexpr T value = T(3.14159265358979L);
};

float a = pi_traits<float>::value;
```

Two lines of access (the type, then `::value`). Acceptable
in metaprogramming but verbose in arithmetic code.

The C++14 variable template is the clean fix: just
`pi<float>`.

### Specialisation works the same as for class/function templates

```cpp
template <class T>
constexpr T zero = T(0);

template <>
constexpr const char* zero<const char*> = "0";   /* explicit specialisation */
```

You can also partial-specialise:

```cpp
template <class T, class U>
constexpr bool same = false;

template <class T>
constexpr bool same<T, T> = true;     /* partial specialisation */
```

`same<int, int>` is `true`; `same<int, double>` is `false`.
This is how `std::is_same_v` (the C++17 alias for
`is_same::value`) is implemented under the hood -- and how
much of the `_v` suite in `<type_traits>` is built.

### `constexpr` is optional but usual

Variable templates don't HAVE to be `constexpr`. You can
declare a non-const variable template:

```cpp
template <class T>
T storage_for_t{};      /* one global per T, mutable */
```

But the canonical use is `constexpr T` so the value is
usable in constant expressions (template args, array sizes,
case labels, ...).

### One instantiation per (template, T) pair

Each `(template, T)` pair gets ONE variable across the whole
program. `pi<double>` is the same `double` object in every
TU. The compiler/linker handle ODR for variable templates
the same way they handle function templates.

### Type deduction

The variable template's TYPE is the declared type `T` --
fully written, no `auto` shortcut. If the initialiser is a
different type, it's converted. `pi<float>` declared as
`constexpr T pi = T(3.14159265358979323846L)`:

- `T` is `float`.
- The literal `3.14159...L` is `long double`.
- `T(literal)` is `float(long double)` -- narrowing
  conversion.
- The variable's type is `const float`.

The narrowing is fine here because we're explicit about it
(`T(...)`). A direct list-initialisation `T x{literal}`
would error on narrowing.

### Common standard library use

`<type_traits>`'s `_v` suite (C++17):

```cpp
template <class T>
constexpr bool is_integral_v = is_integral<T>::value;
```

This is the C++17 alias added on top of C++14's variable
template feature. Same idea: short, direct, no `::value`.

### When NOT to use variable templates

- For computed values that depend on multiple args -- a
  function template is often clearer.
- For values that need to be MUTABLE per instantiation --
  that's a "global per type" pattern, which is rarely what
  you want.

For pure constants parameterised by type, this is the
correct tool.

## Task

In `solution.hpp`, fill in the initialisers for two variable
templates:

```cpp
namespace task07
{
    template <class T>
    constexpr T pi = T(3.14159265358979323846L);

    template <class T>
    constexpr T e = T(2.71828182845904523536L);
}
```

The `T(literal_L)` pattern: write the most-precise literal
(long double, the `L` suffix), then narrow to `T` via
explicit conversion. This way `pi<float>` gets a `float`-
precision value, `pi<double>` gets a `double`-precision
value, and `pi<long double>` keeps the full precision.

## Run

```sh
./verify.sh
```

## Hints

- Use the `L` suffix on the literal so it's a long double
  first, then narrowed to T. This preserves precision for
  `T = long double` and gives identical-up-to-narrowing
  values for the smaller types.

- `T(...)` is a functional-cast / explicit conversion.
  Equivalent to `static_cast<T>(...)` for arithmetic types.

- The pi reference value:
  `3.14159265358979323846L`
  (20 significant digits, all of which fit in long double on
  x86 -- some are dropped on platforms where long double
  == double).

- The e reference value:
  `2.71828182845904523536L`.

- Both variable templates are `constexpr` -- usable at
  compile time. Tests verify this with `constexpr double p =
  pi<double>`.

- The float-vs-double precision loss test:
  `static_cast<double>(pi<float>) - pi<double>` should be
  ~1e-8 (the gap between float and double precision for pi).
  Stub returns 0 - 0 = 0, fails the `EXPECT_GT(diff, 1e-9)`.

- Tests verify: value-near for pi and e at float / double /
  long double, type checks (decltype is `const T` for each
  T), compile-time constexpr evaluation, the float-precision
  loss vs double, and an identity check (reading the same
  variable twice gives the same value).

- For C++17 inline variable templates (a related but distinct
  feature) -- not part of C++14. The form here is the
  standard C++14 variable template.
