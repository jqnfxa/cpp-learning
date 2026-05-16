# 16 — `range-for` over containers

> Spec: <https://en.cppreference.com/cpp/language/range-for>

## What you learn

Range-for works on anything that has `begin()` and `end()` returning
iterators — every standard container, `std::string`, `std::array`,
`std::initializer_list`, and any user-defined type that provides
those member functions (or free `begin(x)` / `end(x)` overloads).

For `std::vector<T>`, that's just:

```cpp
std::vector<int> v = {1, 2, 3};
for (int x : v)
{
    /* x is each element of v in turn */
}
```

You don't need the templated `(&arr)[N]` dance from step 15 —
containers don't decay. A plain `const std::vector<int> &` is fine.

The pattern *underneath* range-for is the standard `begin()`/`end()`
iterator pair:

```cpp
for (auto it = v.begin(); it != v.end(); ++it)
{
    auto &x = *it;
    /* ... */
}
```

Range-for is just the compiler writing that loop for you, with a
hidden iterator name.

## Task

In `solution.hpp`, implement two functions inside `task16`:

```cpp
int sum(const std::vector<int> &v);
std::string join(const std::vector<std::string> &parts,
                 const std::string &sep);
```

Both must walk their input with a `range-for` loop.

| Call                                              | Returns       |
|---------------------------------------------------|---------------|
| `sum({1, 2, 3, 4, 5})`                            | `15`          |
| `sum({})`                                         | `0`           |
| `sum({-3, 5, -2, 4})`                             | `4`           |
| `join({"a", "b", "c"}, ",")`                      | `"a,b,c"`     |
| `join({}, ",")`                                   | `""`          |
| `join({"only"}, ",")`                             | `"only"`      |
| `join({"x", "y", "z"}, " -- ")`                   | `"x -- y -- z"` |
| `join({"a", "b", "c"}, "")`                       | `"abc"`       |

Notice `join` puts separators **between** elements — not before, not
after. The very first element has nothing before it; the very last
has nothing after it.

## Run

```sh
./verify.sh
```

## Hints

- `sum` is the same shape as in step 15. Range-for over a vector
  reference works without templates.
- For `join`, the cleanest way to handle "between, not around" is a
  flag:
  ```cpp
  std::string result;
  bool first = true;
  for (const std::string &s : parts)
  {
      if (!first) result += sep;
      result += s;
      first = false;
  }
  return result;
  ```
  A different idiom: append `sep + s` every time *except* on the
  first iteration. Either works.
- Use `const std::string &` (or `auto const &`) for the loop variable
  to avoid copying each string per iteration. Same shape applies to
  any expensive-to-copy element type. Step 17 explores when this
  matters.
