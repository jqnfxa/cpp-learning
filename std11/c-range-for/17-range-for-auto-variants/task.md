# 17 — `range-for` with `auto &` vs `auto const &` vs `auto`

> Spec: <https://en.cppreference.com/cpp/language/range-for>

## What you learn

The loop variable in a range-for declares **a new variable per
iteration**, initialized from the dereferenced iterator. The three
shapes you reach for in practice are:

| Declaration         | What `x` is              | Use when                          |
|---------------------|--------------------------|-----------------------------------|
| `for (T x : c)`     | a **copy** of each element | element is cheap to copy *and* you might want to mutate the local without touching the container |
| `for (T &x : c)`    | a **reference** to each element | you want to mutate the container's elements |
| `for (T const &x : c)` | a **const reference** to each element | read-only and you want to avoid the copy |

With `auto` deduction these become `auto x`, `auto &x`, `auto const &x`.

The pitfall is using the wrong one:

- **`auto x` over expensive elements** silently copies. A loop over a
  `vector<std::string>` doing `for (auto x : v)` allocates a new
  string per iteration. Switch to `auto const &x`.

- **`auto x` when you expected to mutate** does nothing visible.
  ```cpp
  std::vector<int> v = {1, 2, 3};
  for (auto x : v) x *= 2;        /* v is still {1, 2, 3} */
  for (auto &x : v) x *= 2;       /* v is now {2, 4, 6} */
  ```

- **`auto &x` when you only read** is harmless but slightly inferior
  to `auto const &x`: the latter documents intent and lets the
  compiler refuse accidental writes.

## Task

Implement three functions inside `task17`, each picking the right
range-for variable form for its job:

```cpp
void double_in_place(std::vector<int> &v);
int  try_double_via_copy(std::vector<int> &v);   /* returns the doubled sum, leaves v alone */
std::size_t count_long_strings(const std::vector<std::string> &v, std::size_t min_length);
```

| Function                                                  | Pick                | What it does                                |
|-----------------------------------------------------------|---------------------|---------------------------------------------|
| `double_in_place(v)`                                      | `auto &x`           | multiplies each element by 2 *in place*     |
| `try_double_via_copy(v)`                                  | `auto x`            | mutates the **copies** (not v); returns the sum of the doubled copies |
| `count_long_strings(v, min_length)`                       | `auto const &s`     | returns how many strings have `.size() >= min_length`, no copies of the string |

The pair `double_in_place` / `try_double_via_copy` is the
demonstration: they look almost identical inside the loop, but the
first mutates the container while the second only mutates throwaway
copies.

## Run

```sh
./verify.sh
```

## Hints

- `double_in_place`:
  ```cpp
  for (auto &x : v) x *= 2;
  ```
- `try_double_via_copy`:
  ```cpp
  int total = 0;
  for (auto x : v) { x *= 2; total += x; }
  return total;
  ```
  Crucially, `v` itself does **not** change. The tests assert that
  exactly.
- `count_long_strings`:
  ```cpp
  std::size_t n = 0;
  for (auto const &s : v) if (s.size() >= min_length) ++n;
  return n;
  ```
- The whole point is that you *could* use `auto &` everywhere or
  `auto` everywhere and still produce something runnable. The task
  is to *pick the right one* per situation.
