# 101 — property traits: `is_integral`, `is_floating_point`, `is_pointer`

> Spec: <https://en.cppreference.com/cpp/header/type_traits>

## What you learn

`<type_traits>` is C++11's compile-time-reflection library.
Hundreds of metafunctions answer questions like "is T an
integer?", "is T a class?", "is T derived from U?". They split
into two halves:

| Family | Examples | Returns |
|--------|----------|---------|
| **Property traits**       | `is_integral<T>`, `is_pointer<T>` | a `::value` (bool) |
| **Transformation traits** | `remove_reference<T>`, `decay<T>` | a `::type` (a type) |

Step 101 is property traits. Step 102 covers transformation
traits.

### The three classifiers in this task

```cpp
std::is_integral<int>::value         /* true */
std::is_integral<bool>::value         /* true (bool is integral!) */
std::is_integral<char>::value         /* true */
std::is_integral<float>::value        /* false */

std::is_floating_point<double>::value /* true */
std::is_floating_point<long>::value   /* false */

std::is_pointer<int *>::value         /* true */
std::is_pointer<int>::value           /* false */
std::is_pointer<std::nullptr_t>::value /* false (special) */
```

The three categories are *non-overlapping*: a given `T` is at
most one of integral / floating_point / pointer.

### Two access syntaxes

Each property trait is a class template with a static
`::value` of type `bool`:

```cpp
if (std::is_integral<T>::value) { ... }      /* the long form */
```

C++14 added the `_v` suffix shortcut (`std::is_integral_v<T>`)
to skip the `::value`. C++17 made it standard practice. For
C++11 you write the full `::value` access.

### Why does `bool` count as integral?

The C++ type system splits arithmetic types as:

- **Integral**: `bool`, `char`, `signed char`, `unsigned char`,
  `wchar_t`, `char16_t`, `char32_t`, `short`, `int`, `long`,
  `long long`, plus their unsigned cousins, and any cv-
  qualified versions.
- **Floating-point**: `float`, `double`, `long double` (and cv).

So `is_integral<bool>::value` is `true`. This is sometimes
surprising; reach for `is_signed`, `is_unsigned`, or
`is_arithmetic` when you specifically don't want booleans
included in your "is integer?" check.

### `is_pointer` and friends

`is_pointer<T>` returns `true` for *raw* pointer types only:
`int *`, `const char *`, `void *`, function pointers. It does
**not** return `true` for:
- `std::nullptr_t` (use `is_null_pointer` in C++14).
- Member function pointers (`is_member_function_pointer`).
- `std::shared_ptr<T>`, `std::unique_ptr<T>` (those are class
  types).
- Iterators that happen to be pointers (e.g. `vector<int>`'s
  raw-pointer-iterator on libstdc++ -- the trait reports the
  underlying type).

For "looks like a pointer in source", `is_pointer` is what you
want.

### Using property traits

The classic patterns:

```cpp
/* tag dispatch */
template <class T> void dispatch(T x, std::true_type)  { /* integral */ }
template <class T> void dispatch(T x, std::false_type) { /* not */ }

template <class T> void f(T x) {
    dispatch(x, std::is_integral<T>{});
}
```

```cpp
/* enable_if (step 103) */
template <class T,
          class = typename std::enable_if<std::is_integral<T>::value>::type>
void only_for_integers(T x);
```

```cpp
/* static_assert */
template <class T>
void must_be_ptr(T) {
    static_assert(std::is_pointer<T>::value, "T must be a raw pointer");
}
```

The property traits give you the predicate; the rest of
template metaprogramming uses them.

## Task

In `solution.hpp`, write one function template that classifies
a type:

```cpp
namespace task101
{
    /* Return "integral", "floating_point", "pointer", or "other"
     * based on T's category. */
    template <class T>
    std::string name_kind();
}
```

The body is a chain of `if` statements over the three traits.

## Run

```sh
./verify.sh
```

## Hints

- The body is direct:
  ```cpp
  if (std::is_integral<T>::value)       return "integral";
  if (std::is_floating_point<T>::value) return "floating_point";
  if (std::is_pointer<T>::value)        return "pointer";
  return "other";
  ```

- The function takes no arguments -- it's parameterised on `T`
  only. Callers use it as `name_kind<int>()`,
  `name_kind<double>()`, etc.

- A `constexpr` version of this function would also be valid,
  but in C++11 `constexpr` functions have a single-return-
  statement constraint that makes this style awkward. C++14
  relaxes it; we stick to the plain runtime version here.

- `if constexpr` (C++17) would let you dispatch at compile time
  without `if`/`return` chains. Not part of C++11.

- The chain order matters slightly: integral / floating_point /
  pointer are mutually exclusive, so the chain's correctness
  doesn't depend on order. Convention is to write them in the
  order they appear in `<type_traits>` (integral first).

- The three classes here are part of the larger "primary type
  categories" set: `is_void`, `is_null_pointer`,
  `is_integral`, `is_floating_point`, `is_array`, `is_pointer`,
  `is_lvalue_reference`, `is_rvalue_reference`,
  `is_member_object_pointer`, `is_member_function_pointer`,
  `is_enum`, `is_union`, `is_class`, `is_function`. Together
  these exhaustively partition every C++ type into one and
  only one category.
