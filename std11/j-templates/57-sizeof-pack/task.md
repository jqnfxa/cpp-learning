# 57 — `sizeof...(Pack)`

> Spec: <https://en.cppreference.com/cpp/language/parameter_pack>

## What you learn

`sizeof...` is the **count operator** for parameter packs. It
takes the name of a pack (either a type pack like `Ts` or a
function-argument pack like `args`) and returns a
`std::size_t` *constant expression* equal to the number of
elements.

```cpp
template <class... Ts>
constexpr std::size_t how_many(const Ts &...args)
{
    static_assert(sizeof...(Ts) == sizeof...(args),
                  "the two packs always agree");
    return sizeof...(Ts);    /* same as sizeof...(args) */
}
```

Notes:

1. **Operand is the pack *name*, not an expansion.** You write
   `sizeof...(Ts)` (or `sizeof...(args)`), not `sizeof...(Ts...)`.

2. **Result is a `std::size_t` constant expression.** Usable as a
   template argument, in `static_assert`, as an array bound.

3. **You can mix it with normal arithmetic.** `sizeof...(Ts) > 0`,
   `sizeof...(Ts) - 1`, etc., are all fine. Just don't mistake it
   for `sizeof(...)` (no `...` between operand and parens).

## Task

In `solution.hpp`, implement two function templates and a
class-template member:

```cpp
template <class... Ts>
constexpr std::size_t count_args(const Ts &.../* args */);

template <class... Ts>
constexpr std::size_t count_types();

template <class... Ts>
struct type_list
{
    static constexpr std::size_t size = sizeof...(Ts);
};
```

| Call                                                | Returns |
|-----------------------------------------------------|---------|
| `count_args()`                                      | `0`     |
| `count_args(1)`                                     | `1`     |
| `count_args(1, "x", 3.14)`                          | `3`     |
| `count_types<>()`                                   | `0`     |
| `count_types<int>()`                                | `1`     |
| `count_types<int, double, char>()`                  | `3`     |
| `type_list<>::size`                                 | `0`     |
| `type_list<int>::size`                              | `1`     |
| `type_list<int, double, char, std::string>::size`   | `4`     |

## Run

```sh
./verify.sh
```

## Hints

- One-line bodies:
  ```cpp
  template <class... Ts>
  constexpr std::size_t count_args(const Ts &.../* args */)
  {
      return sizeof...(Ts);
  }
  ```

- `sizeof...(Ts)` and `sizeof...(args)` produce the same value.
  Pick whichever spells your intent best. Type-only contexts (no
  args) require `sizeof...(Ts)`.

- The `type_list::size` member is `constexpr static`. Define it
  in the class body the same way you'd define any
  `static constexpr` member.

- C++17 inline variables let you avoid an out-of-class definition
  for `static constexpr` members. In C++11, the in-class
  initializer counts as a *declaration*; if you take its address
  (or odr-use it), you also need an out-of-class definition. The
  tests in `tests.cpp` never odr-use `::size`, so the in-class
  initializer alone is enough.
