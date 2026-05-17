# 99 — `std::get<I>(tuple)` and `std::tie`

> Spec: <https://en.cppreference.com/cpp/utility/tuple/get>,
> <https://en.cppreference.com/cpp/utility/tuple/tie>

## What you learn

C++11's pre-structured-bindings way of "destructure a tuple into
named locals" is the combination of:

- `std::get<I>(t)` — read one element by position.
- `std::tie(a, b, c)` — build a *tuple of references* and
  assign into it.

The two together cover all the unpacking idioms that
`auto [a, b, c] = t;` (C++17) was eventually added to express.

### `std::tie` for assignment

`std::tie(a, b, c)` returns a `std::tuple<int &, std::string &,
double &>` — a tuple whose elements are *references* to the
arguments. When you assign another tuple to it, the element-wise
assignment goes through the references and lands in the
original variables:

```cpp
int         id;
std::string name;
double      score;

std::tie(id, name, score) = make_record(...);
/* id, name, score now hold the components of the record */
```

This is the C++11 "structured binding" idiom: one line of
assignment unpacks the entire tuple into pre-declared locals.
The price is that you have to declare the locals first
(`auto [id, name, score] = ...` does it in one shot in
C++17, but doesn't exist in C++11).

### `std::tie` for the multi-assign swap

A surprising side use: the classic swap-via-tuple:

```cpp
int a = 1, b = 2;
std::tie(a, b) = std::make_tuple(b, a);
/* a == 2, b == 1 */
```

This works because the right-hand side is evaluated first
(producing `(2, 1)`), then the assignment copies both into the
referenced locals. The same pattern generalises to
"reassign N variables simultaneously from N expressions of the
old values" without temporaries.

(`std::swap(a, b)` is still the right tool for two-variable
swap; the tie form generalises to N.)

### `std::ignore`

When you don't care about some tuple slot:

```cpp
int id;
std::tie(id, std::ignore, std::ignore) = record;
/* only `id` is assigned; other slots are read into a null sink */
```

`std::ignore` is a magic global that absorbs any assignment.

### Lexicographic comparison short-cut

`std::tie` plus comparison gives you the cleanest possible
multi-key comparator:

```cpp
struct Employee { int dept; double salary; std::string name; };

bool sort_by_dept_then_salary(const Employee &a, const Employee &b)
{
    return std::tie(a.dept, a.salary, a.name)
         < std::tie(b.dept, b.salary, b.name);
}
```

Tuple `operator<` is lexicographic by default. `std::tie` lets
you build that comparator without copying the fields.

## Task

In `solution.hpp`, implement two helpers over the same
`record` tuple from step 98:

```cpp
namespace task99
{
    using record = std::tuple<int, std::string, double>;

    /* Look up an element by position. */
    int          first_field(const record &r);
    std::string  second_field(const record &r);
    double       third_field(const record &r);

    /* Unpack r into three lvalue references using std::tie. */
    void unpack(const record &r, int &id, std::string &name, double &score);
}
```

The three `*_field` functions are one-line wrappers around
`std::get<0>` / `<1>` / `<2>`. `unpack` is the
`std::tie(id, name, score) = r;` idiom.

## Run

```sh
./verify.sh
```

## Hints

- `first_field`, `second_field`, `third_field`:
  ```cpp
  return std::get<0>(r);
  return std::get<1>(r);
  return std::get<2>(r);
  ```

- `unpack` body:
  ```cpp
  std::tie(id, name, score) = r;
  ```
  One line. The references in the LHS tuple absorb the assignment
  element-by-element.

- `std::tie` is in `<tuple>`. Include it.

- `second_field` returns `std::string` by value (copy). If the
  test is hot and you want to avoid the copy, you'd return a
  const reference instead. For the curriculum, by-value is the
  simpler contract.

- The same `std::tie` machinery powers `std::pair` unpacking
  too: `std::tie(x, y) = std::make_pair(1, 2);` works
  identically.

- C++17 structured bindings would shorten the `unpack`
  caller to:
  ```cpp
  auto [id, name, score] = make_record(...);
  ```
  but that requires C++17. The `tie` idiom is what you use in
  C++11/14.

- `std::ignore` is not part of this task, but worth knowing for
  "unpack and discard some slots" cases.
