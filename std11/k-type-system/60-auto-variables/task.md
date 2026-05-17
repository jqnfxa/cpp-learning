# 60 — `auto` for variables

> Spec: <https://en.cppreference.com/cpp/language/auto>

## What you learn

`auto` in a variable declaration tells the compiler "deduce my
type from the initializer". You still get a *specific* type — it's
just spelled by the compiler instead of you.

```cpp
auto x = 42;                /* int */
auto y = 3.14;              /* double */
auto v = std::vector<int>{1, 2, 3};   /* std::vector<int> */
auto it = v.begin();        /* std::vector<int>::iterator */
```

The classic use case is iterators, where the type name is long and
the deduction is obvious from context:

```cpp
/* Before: spelled iterator type. */
for (std::vector<std::pair<int, std::string>>::const_iterator it = v.begin();
     it != v.end(); ++it)
{ ... }

/* After: auto, but otherwise identical. */
for (auto it = v.begin(); it != v.end(); ++it)
{ ... }
```

`auto` is C++11's first big quality-of-life win. It composes
naturally with the new container types and lambda return types,
which would otherwise be unspellable.

## What `auto` deduces

`auto` deduction follows the **same rules as template type
deduction** (`template <class T> void f(T x);`). The most common
results:

| Init                       | Deduced `auto` type |
|----------------------------|---------------------|
| `auto x = 42;`             | `int`               |
| `auto x = "hi";`           | `const char *`      |
| `int n = 5; auto x = n;`   | `int` (copy)        |
| `int &r = ...; auto x = r;` | `int` (copy, not ref!) |
| `auto &x = ...;`           | `T &` (reference preserved) |
| `auto const &x = ...;`     | `const T &` |
| `auto x = {1, 2, 3};`      | `std::initializer_list<int>` |

The "drops references" rule is the trap. If you want a reference,
write `auto &` explicitly.

## Task

In `solution.hpp`, implement two functions that internally use
`auto` for variable types:

```cpp
/* Sum the elements of v using `auto` for the iterator. */
int sum_via_iterator(const std::vector<int> &v);

/* Find the first index of `needle` (or v.size() if not present)
 * using `auto` for the iterator. */
std::size_t find_index(const std::vector<int> &v, int needle);
```

The tests can only check observable behavior — they can't see
your `auto` declarations. You're on the honor system to use
`auto` where it makes sense (and not use it where it doesn't).

| Call                                         | Returns |
|----------------------------------------------|---------|
| `sum_via_iterator({1, 2, 3, 4, 5})`          | `15`    |
| `sum_via_iterator({})`                       | `0`     |
| `sum_via_iterator({-3, 5, -2, 4})`           | `4`     |
| `find_index({10, 20, 30, 20}, 20)`           | `1`     |
| `find_index({10, 20, 30}, 99)`               | `3` (== v.size()) |
| `find_index({}, 1)`                          | `0`     |

## Run

```sh
./verify.sh
```

## Hints

- `sum_via_iterator` with explicit iterator:
  ```cpp
  int total = 0;
  for (std::vector<int>::const_iterator it = v.begin();
       it != v.end(); ++it)
  {
      total += *it;
  }
  return total;
  ```
  With `auto`, the `for` head shrinks to
  `for (auto it = v.begin(); it != v.end(); ++it)`.

- `find_index` returns `std::size_t`. Use `auto it` from
  `std::find(v.begin(), v.end(), needle)`, then
  `static_cast<std::size_t>(it - v.begin())`.

- Don't use range-for here — you'd lose the iterator handle that
  `find_index` needs for the position calculation. (Range-for has
  its own task, step 15.)

- `auto` deduction may surprise you in templates that return
  references. `auto x = some_ref()` strips the reference; `auto
  &x = some_ref()` preserves it. The default — no `&` — is "copy".
