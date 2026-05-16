# 18 — List initialization (uniform `T{}`)

> Spec: <https://en.cppreference.com/cpp/language/list_initialization>

## What you learn

Before C++11, initialization syntax depended on the type:

```cpp
int x = 5;                    /* copy initialization */
int x(5);                     /* direct initialization */
std::vector<int> v(3, 0);     /* fill ctor */
std::vector<int> v(begin, end); /* range ctor */
MyStruct s = { 1, 2.5 };      /* aggregate init only */
```

Different shapes for different kinds of types. C++11 introduced
**list initialization** — one form, `T{ ... }`, that works
*everywhere*:

```cpp
int x{5};
std::vector<int> v{1, 2, 3};
std::pair<int, std::string> p{42, "hello"};
MyStruct s{1, 2.5};
T t{};                        /* value-initialized */
```

The compiler picks the right underlying mechanism based on `T`:
- For fundamental types: direct initialization.
- For aggregates (plain structs / arrays): aggregate initialization,
  member-by-member.
- For types with an `initializer_list` constructor (`std::vector`,
  `std::map`, ...): that constructor is preferred.
- Otherwise: best-match ordinary constructor.

Empty braces, `T{}`, are **value initialization** — zero-initialized
for fundamental and POD types, default-constructed for class types
with user-provided defaults.

The brace form also has one extra property worth knowing: it
**prevents narrowing conversions**. `int x{3.5}` is a compile error.
`int x(3.5)` silently truncates to 3. That's the topic of step 19.

## Task

In `solution.hpp`, implement four functions inside `task18`, each
returning a value built with the `T{...}` brace form.

```cpp
int make_int(int n);
int make_default_int();
std::pair<int, int> make_pair_of(int a, int b);
std::vector<int> make_vec3(int a, int b, int c);
```

| Function                          | Implementation hint                                |
|-----------------------------------|----------------------------------------------------|
| `make_int(n)`                     | `return int{n};`                                   |
| `make_default_int()`              | `return int{};` (value-initialized: result is 0)   |
| `make_pair_of(a, b)`              | `return std::pair<int, int>{a, b};`                |
| `make_vec3(a, b, c)`              | `return std::vector<int>{a, b, c};`                |

Tests check the values returned by each function, and there's one
extra test that pins `make_default_int()` to exactly `0` (the
value-initialization of `int`).

## Run

```sh
./verify.sh
```

## Hints

- `int{}` is `0`. The compiler value-initializes the fundamental
  type, which means zero-initialization.
- `std::vector<int>{a, b, c}` calls the
  `std::initializer_list<int>` constructor with three elements.
  Contrast with `std::vector<int>(3, 0)` (size 3, all zero) — the
  parens form picks the *fill* constructor.
- You *could* return `std::pair<int, int>(a, b)` (parens form), but
  this task is specifically about the **brace form**. Same for
  vector.
- The braces around `int{n}` are technically redundant when `n` is
  already an `int` (you could just write `return n;`), but here the
  point is to exercise the syntax.
