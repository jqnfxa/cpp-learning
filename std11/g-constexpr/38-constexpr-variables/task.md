# 38 — `constexpr` variables

> Spec: <https://en.cppreference.com/cpp/language/constexpr>

## What you learn

C++11's `constexpr` is the language-level promise "this value is
available at *compile time*". Two flavors exist:

1. **constexpr variables** (this step): values fixed and computable
   at compile time. Usable as array bounds, template arguments,
   non-type template parameters, `static_assert` conditions, and
   `case` labels — every place that pre-C++11 required `const int`
   or `enum`-based hacks.

   ```cpp
   constexpr int k_size = 16;
   int buf[k_size];                    /* OK -- array bound is a constant expression */
   static_assert(k_size == 16, "");    /* OK */
   std::array<int, k_size> arr;        /* OK -- template arg */
   ```

2. **constexpr functions** (step 39): functions usable in constant
   expressions when called with constant arguments. With the
   strict C++11 rules: single return statement, no locals, no
   `if`/`for`. Step 39 is the function half.

`constexpr` vs `const` on variables:

- `const int x = f();` — read-only after init. `x` may or may not
  be a constant expression (depends on whether `f()` is also
  `constexpr` and the value is computable at compile time).
- `constexpr int x = f();` — read-only AND guaranteed to be a
  constant expression. `f()` must be `constexpr`. If the compiler
  can't compute `x` at compile time, you get an error.

In short, `constexpr` is a stronger contract.

## Task

In `solution.hpp`, declare four `constexpr` variables and use them
in places that require constant expressions:

```cpp
constexpr int k_size = 16;
constexpr std::size_t k_buffer_capacity = 1024;
constexpr double k_pi = 3.14159265358979323846;
constexpr int k_max_retries = 3;
```

Then declare a single function that uses one of them as an array
bound, so the tests can interact with it:

```cpp
/* Returns a pointer to a static array of k_size ints. */
const int *get_buffer();
```

Implementation:

```cpp
inline const int *get_buffer()
{
    static int buf[k_size]{};   /* k_size used as a constant-expression array bound */
    return buf;
}
```

Tests verify:
- Each constexpr variable has the expected value (runtime).
- Each is usable in a `static_assert` (compile time).
- `k_size` works as an array bound: `int arr[task38::k_size]`
  compiles and has size `16`.
- `k_buffer_capacity` works as a template argument:
  `std::array<int, task38::k_buffer_capacity>` instantiates.

## Run

```sh
./verify.sh
```

## Hints

- Each declaration is one line:
  ```cpp
  constexpr int k_size = 16;
  ```
  `constexpr` implies `const` for variables — you don't need both.

- `<cstddef>` provides `std::size_t`. `<array>` provides
  `std::array<T, N>`.

- The naming convention `k_<thing>` is a common one for compile-time
  constants (Google style is `kThing` PascalCase; bigmath uses
  `snake_case`). Pick whichever you like, but match the names the
  tests reference.

- `get_buffer` is provided as a stub that returns `nullptr`. Replace
  it with the static-array implementation. The function body
  declares a `static int buf[k_size]{}` — `k_size` here is a
  *constant expression*, which is exactly what an array bound needs.
