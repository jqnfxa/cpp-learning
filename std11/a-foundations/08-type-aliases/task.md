# 08 — Type aliases with `using`

> Spec: <https://en.cppreference.com/cpp/language/type_alias>

## What you learn

C++11 introduced a second syntax for type aliases:

```cpp
using name = type;
```

This is *almost* equivalent to:

```cpp
typedef type name;
```

The two declarations produce indistinguishable types. The differences
are practical:

1. **Reads left-to-right.** `using u64 = unsigned long long;` says
   "u64 is unsigned long long" in the order you'd say it out loud.
   `typedef unsigned long long u64;` says "alias the long long unsigned
   thing as u64" — readable but inverted.

2. **Function-pointer aliases stop being awful.** Compare:

   ```cpp
   typedef int (*binop)(int, int);    /* name buried in the middle */
   using binop = int (*)(int, int);   /* name on the left, signature on the right */
   ```

3. **`using` composes with templates. `typedef` does not.** Step 09 is
   the alias-template exercise. It's only possible because of `using`:

   ```cpp
   template <class T> using vec = std::vector<T>;   /* legal, alias template */
   /* typedef std::vector<T> vec<T>;                 -- there is no syntax for this */
   ```

`using` is the modern default. Reach for `typedef` only when you're
maintaining old code that already uses it.

## Task

In `solution.hpp`, declare four aliases inside `namespace task08`
using `using` (not `typedef`):

| Alias name       | Aliases                                |
|------------------|----------------------------------------|
| `u64`            | `unsigned long long`                   |
| `int_binary_op`  | `int (*)(int, int)` — pointer to a function taking two `int`s and returning `int` |
| `string_vec`     | `std::vector<std::string>`             |
| `vec_int_iter`   | `std::vector<int>::iterator`           |

`tests.cpp` uses `std::is_same` to assert each alias resolves to the
expected type. With the four names missing, the test file doesn't even
compile.

## Run

```sh
./verify.sh
```

## Hints

- The grammar is `using <name> = <type>;`. Semicolon required.
- For the function-pointer one, the right-hand side is exactly what
  you'd put in a variable declaration: `int (*)(int, int)`. The
  parentheses around `*` are mandatory and disambiguate from
  `int * (int, int)` (a function returning `int*`).
- `vec_int_iter` doesn't need `typename`. The right-hand side is a
  non-dependent name (no template parameters in this scope), so the
  compiler can see it's a type directly.
