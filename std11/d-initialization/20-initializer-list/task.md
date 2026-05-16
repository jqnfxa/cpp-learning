# 20 — `std::initializer_list`

> Spec: <https://en.cppreference.com/cpp/utility/initializer_list>

## What you learn

C++11 introduced `std::initializer_list<T>` — a lightweight,
non-owning view over a temporary array of `T`s. Its main job is to
let you write **brace-list arguments**:

```cpp
int s = sum({1, 2, 3, 4, 5});
auto v = std::vector<int>{1, 2, 3};
```

In both cases the `{...}` becomes a `std::initializer_list<T>` that
the receiving function or constructor consumes.

Properties to remember:

1. **It's a view.** The list owns nothing; it points into a temporary
   `T[]`. Don't store the list past the full-expression that created
   it — the underlying array vanishes.
2. **Iteration is `begin()` / `end()` / `size()`.** Range-for works.
3. **Pass by value, not by reference.** The list itself is two
   pointers; copying it is free. Idiomatic signatures look like
   `void f(std::initializer_list<int> xs)`, no `const &`.
4. **It plays favorites with constructors.** If a class has an
   `initializer_list<T>` constructor *and* other constructors, the
   `{...}` form prefers the initializer_list one. `std::vector{3, 0}`
   constructs a vector `[3, 0]`, not `[0, 0, 0]` (the fill form is
   the paren version).

## Task

In `solution.hpp`, implement three functions inside `task20`:

```cpp
int sum(std::initializer_list<int> values);
int max_of(std::initializer_list<int> values);            /* undefined if empty */
std::vector<int> to_vector(std::initializer_list<int> values);
```

| Call                              | Returns                |
|-----------------------------------|------------------------|
| `sum({1, 2, 3})`                  | `6`                    |
| `sum({})`                         | `0`                    |
| `sum({-5, 10, -3})`               | `2`                    |
| `max_of({3, 1, 4, 1, 5, 9, 2, 6})` | `9`                   |
| `max_of({42})`                    | `42`                   |
| `to_vector({10, 20, 30})`         | `vector<int>{10,20,30}`|
| `to_vector({})`                   | `empty vector`         |

`max_of` is documented as undefined when the list is empty — don't
write defensive logic for that. Tests will only call it with
non-empty input.

## Run

```sh
./verify.sh
```

## Hints

- For `sum`, range-for the list:
  ```cpp
  int total = 0;
  for (int x : values) total += x;
  return total;
  ```
- For `max_of`, the simplest form uses `std::max_element`:
  ```cpp
  return *std::max_element(values.begin(), values.end());
  ```
  Or a manual range-for that tracks the running max.
- For `to_vector`, `std::vector` has a constructor that takes an
  iterator pair. Or even simpler, it has one that takes
  `std::initializer_list<T>` directly — but using your function's
  parameter as the argument:
  ```cpp
  return std::vector<int>(values.begin(), values.end());
  /* or */
  return std::vector<int>(values);
  ```
- All three of these functions exist in `<algorithm>` /
  `<numeric>` in some form, but writing them yourself is the point
  of this step.
