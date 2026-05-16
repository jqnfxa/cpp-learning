# 09 — Alias templates

> Spec: <https://en.cppreference.com/cpp/language/type_alias>

## What you learn

In step 08 you saw that `using` declares a type alias. The payoff of
that new syntax — the thing `typedef` can't do — is that `using`
composes with templates. The result is the **alias template**:

```cpp
template <class T> using vec = std::vector<T>;
vec<int> v;                /* same type as std::vector<int> */
```

`typedef` has no equivalent. There is no syntax that lets you write
"a typedef that takes a template parameter" — you'd have to write a
class template and put a `typedef` inside it (the C++03 workaround,
sometimes called *type-traits style*).

```cpp
/* The C++03 workaround. Five lines, plus you have to write ::type. */
template <class T>
struct vec {
    typedef std::vector<T> type;
};
vec<int>::type v;          /* awkward */
```

C++11 alias templates collapse that to one line and drop the `::type`
noise.

Alias templates have all the parameter-kind flexibility of class
templates:
- type parameters (`class T` / `typename T`),
- non-type parameters (`std::size_t N`),
- multiple parameters in any order.

## Task

In `solution.hpp`, declare four alias templates inside
`namespace task09`:

| Alias                         | Aliases                                  |
|-------------------------------|------------------------------------------|
| `vec<T>`                      | `std::vector<T>`                         |
| `map<K, V>`                   | `std::map<K, V>`                         |
| `array<T, N>`                 | `std::array<T, N>` *(N is `std::size_t`)*|
| `ptr<T>`                      | `T *`                                    |

`tests.cpp` uses `std::is_same` to assert each alias resolves to the
right type for several instantiations, then exercises each at runtime.

## Run

```sh
./verify.sh
```

## Hints

- The grammar is
  ```cpp
  template <class T> using name = type-expression-using-T;
  ```
  with `class` or `typename` interchangeable for type parameters.
- For `array<T, N>`, the non-type parameter is `std::size_t N`. You
  forward it the same way a class template would:
  ```cpp
  template <class T, std::size_t N> using array = std::array<T, N>;
  ```
- For `ptr<T>`, the RHS is just `T *`. Alias templates don't have to
  refer to standard-library types.
- The four declarations are each one line.
