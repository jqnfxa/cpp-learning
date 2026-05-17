# 103 — `std::enable_if` and SFINAE

> Spec: <https://en.cppreference.com/cpp/types/enable_if>

## What you learn

**SFINAE** (Substitution Failure Is Not An Error) is one of C++'s
oldest template tricks: when the compiler tries to substitute
template arguments into a function signature and the
substitution produces an invalid type, the compiler **silently
removes that template from the overload set** instead of
throwing an error.

C++11's `std::enable_if<Cond, T>` is the helper that turns
SFINAE into a usable selection mechanism:

```cpp
template <bool B, class T = void>
struct enable_if {};                /* primary: no ::type */

template <class T>
struct enable_if<true, T>           /* specialised when B is true */
{
    using type = T;
};
```

When `B` is `true`, `enable_if<B, T>::type` is `T`. When `B` is
`false`, `::type` doesn't exist -- and using it in a template
signature triggers SFINAE: the substitution fails, the
overload is removed, the compiler picks a different one (or
gives a "no matching overload" error if none remain).

### The classic "arithmetic vs other" overload

```cpp
template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, int>::type
to_int(T x)
{
    return static_cast<int>(x);   /* int, double, char, bool, ... */
}

int to_int(const std::string &s)
{
    return std::stoi(s);          /* parse the string */
}
```

- `to_int(42)` -> calls the template, `is_arithmetic<int>` is
  true, `::type` is `int`, overload viable.
- `to_int(3.14)` -> template, `is_arithmetic<double>` is true,
  viable.
- `to_int(std::string("99"))` -> template's substitution
  produces `enable_if<false, int>::type`, which doesn't exist;
  template silently removed; the string overload picks up.

The result: from the caller's perspective there is "one
function" `to_int` that does the right thing for both
arithmetic types and strings.

### Where to put the `enable_if`

Three idiomatic placements; pick the one that fits:

1. **Return type** (used above): `enable_if<...>::type` is the
   return type. Simple, but doesn't let you SFINAE on a
   non-returning function.

2. **Default template argument**:
   ```cpp
   template <class T,
             class = typename std::enable_if<COND>::type>
   void f(T);
   ```
   Works on constructors and void-returning functions, but the
   default-arg-only-considered-once-rule has gotchas with
   identical signatures.

3. **Function parameter default**:
   ```cpp
   template <class T>
   void f(T, typename std::enable_if<COND>::type * = nullptr);
   ```
   Robust against the rule-of-three above; verbose.

For this task we use the return-type form -- shortest, works,
no pitfalls when the two overloads have distinct argument
shapes (`T` vs `const std::string &`).

### Why not just `if constexpr`?

C++17 added `if constexpr` which lets you write:

```cpp
template <class T>
int to_int(const T &x)
{
    if constexpr (std::is_arithmetic_v<T>) return static_cast<int>(x);
    else if constexpr (std::is_convertible_v<T, std::string>) return std::stoi(x);
    else static_assert(/*...*/);
}
```

One function, branches on the trait at compile time. In C++11
you don't have `if constexpr`, so SFINAE-via-enable_if is the
spelling.

### `enable_if_t` (C++14)

C++14 added the `_t` shortcut:

```cpp
std::enable_if_t<COND, int>   /* same as typename std::enable_if<COND, int>::type */
```

In C++11 you write the full `typename std::enable_if<...>::type`.

## Task

In `solution.hpp`, write two `to_int` overloads:

```cpp
namespace task103
{
    /* Selected when T is arithmetic (integral or floating-point).
     * Returns static_cast<int>(x). */
    template <class T>
    typename std::enable_if<std::is_arithmetic<T>::value, int>::type
    to_int(T x);

    /* Non-template overload for std::string. Returns std::stoi(s)
     * (throws std::invalid_argument on bad input). */
    int to_int(const std::string &s);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The template overload's body is `return static_cast<int>(x);`.
  The cast handles int -> int (trivial), double -> int
  (truncates), char -> int (ASCII value), bool -> int (0 or 1).

- The string overload's body is `return std::stoi(s);` from
  `<string>`.

- `std::stoi` throws `std::invalid_argument` when the string
  doesn't start with digits and `std::out_of_range` when the
  parsed integer overflows. The tests catch
  `std::invalid_argument`.

- A string literal `"123"` is `const char[4]`, not
  `std::string`. The arithmetic template won't match (char
  arrays aren't arithmetic). The string overload's parameter is
  `const std::string &` -- an implicit `std::string(const char *)`
  conversion will fire, and `to_int("123")` will call the
  string overload.

- The arithmetic template parameter `T` is taken by *value* in
  this task. For more general code you'd take by `const T &`
  and add `decay` to the trait check; for plain arithmetic
  types, by-value is the natural ABI.

- Don't write the string overload as a template specialisation
  of the arithmetic one. They are two separate functions in
  the same overload set. Specialising would be more confusing
  than helpful here.

- The error message "no matching function to call 'to_int(X)'"
  when X is neither arithmetic nor string-convertible is the
  *intent*: callers should know which types `to_int` accepts.
  Adding a `static_assert(false)` catch-all is sometimes done
  in production code for better diagnostics.
