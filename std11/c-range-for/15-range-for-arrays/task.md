# 15 — `range-for` over arrays

> Spec: <https://en.cppreference.com/cpp/language/range-for>

## What you learn

C++11 introduced the **range-based for loop**:

```cpp
for (auto x : container)
{
    /* x is each element in turn */
}
```

For a C-style array, range-for works when the loop sees the array as
*an array* (not as a pointer-to-first-element). The compiler can
deduce both endpoints from the array type:

```cpp
const int arr[] = {1, 2, 3};

for (int x : arr) { /* x walks 1, 2, 3 */ }      /* OK: arr is int[3] */

const int *p = arr;
for (int x : p) { /* error: p is a pointer, not a range */ }
```

The thing that traps C programmers: arrays *decay* to pointers when
passed to a function. To preserve "this is an array", the function
takes the array **by reference**, which usually means using a
template to keep the size flexible:

```cpp
template <std::size_t N>
int sum(const int (&arr)[N])
{
    int total = 0;
    for (int x : arr) total += x;
    return total;
}
```

The `(&arr)[N]` syntax is "reference to an array of `N` ints". Don't
worry about the templates — that comes in section J. The shape is
provided for you in this task; you just fill in the loop body.

## Task

In `solution.hpp`, implement the bodies of two function templates
inside `task15`:

```cpp
template <std::size_t N>
int sum(const int (&arr)[N]);

template <std::size_t N>
std::size_t count_evens(const int (&arr)[N]);
```

Both must walk the array with a `range-for` loop (not an indexed
`for (size_t i = 0; i < N; ++i)` loop). The point of the task is
exercising range-for; the indexed form would also work, but it isn't
what you're learning.

| Function                              | Returns |
|---------------------------------------|---------|
| `sum({1, 2, 3, 4, 5})`                | `15`    |
| `sum({0})`                            | `0`     |
| `sum({-3, 5, -2, 4})`                 | `4`     |
| `count_evens({1, 2, 3, 4, 5, 6})`     | `3`     |
| `count_evens({1, 3, 5, 7})`           | `0`     |
| `count_evens({2, 4, 6, 8})`           | `4`     |

## Run

```sh
./verify.sh
```

## Hints

- The body of `sum`:
  ```cpp
  int total = 0;
  for (int x : arr) total += x;
  return total;
  ```
- The body of `count_evens`:
  ```cpp
  std::size_t n = 0;
  for (int x : arr) if (x % 2 == 0) ++n;
  return n;
  ```
- Read-only iteration through ints is cheap; `int x` (copy) is fine
  here. The next task explores `auto &` vs `auto const &` vs `auto`
  and when each matters.
