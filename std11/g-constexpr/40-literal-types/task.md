# 40 — Literal types and `constexpr` constructors

> Spec: <https://en.cppreference.com/cpp/named_req/LiteralType>

## What you learn

A **literal type** is a type whose values can appear in *constant
expressions* — usable as `constexpr` variables, template
arguments, `static_assert` operands, and so on. C++11 lays out the
requirements:

A class is a literal type when:
- it has a trivial destructor, *and*
- it is either an aggregate or has at least one `constexpr`
  constructor (other than copy/move), *and*
- all non-static data members and base classes are themselves
  literal types, *and*
- if there's a class member that has a user-declared default ctor,
  every variant member has a brace-or-equal initializer.

In practice for a "regular" small value type — `point`, `rect`,
`color` — the recipe is:

```cpp
class point
{
public:
    constexpr point(int x, int y) : x_(x), y_(y) {}

    constexpr int x() const { return x_; }
    constexpr int y() const { return y_; }

private:
    int x_;
    int y_;
};
```

Three `constexpr`s: the constructor, and each accessor. With those,
you can write:

```cpp
constexpr point origin{0, 0};
constexpr int ox = origin.x();   /* 0 -- evaluated at compile time */
static_assert(origin.x() == 0, "");
```

C++11 `constexpr` constructors are subject to the same rules as
`constexpr` functions (step 39): empty body except for the
member-initializer list. C++14 will lift this.

## Task

In `solution.hpp`, declare `task40::point` with:

```cpp
class point
{
public:
    constexpr point(int x, int y);

    constexpr int x() const;
    constexpr int y() const;

private:
    int x_;
    int y_;
};
```

Plus a free `constexpr` function:

```cpp
constexpr int distance_squared(point a, point b);
/* (a.x - b.x)^2 + (a.y - b.y)^2 */
```

All four functions are `constexpr`. The point type itself is a
literal type because all four members (ctor + 2 accessors are
constexpr; one int data members each are literal; trivial dtor).

`tests.cpp` exercises `point` in three constant-expression contexts:
- `constexpr point p{3, 4};`
- `static_assert(p.x() == 3, "");`
- `static_assert(distance_squared({0,0}, {3,4}) == 25, "");`

## Run

```sh
./verify.sh
```

## Hints

- Ctor:
  ```cpp
  constexpr point(int x, int y) : x_(x), y_(y) {}
  ```
  Empty body is required for a constexpr ctor in C++11. Move all
  initialization into the member-init list.

- Accessors: one liner each, marked `constexpr`. (As of C++11,
  `constexpr` on a member function *implies* `const`. That implicit
  `const` is removed in C++14; with our `-std=c++11` build, keep
  writing `const` for clarity.)

- `distance_squared`: single return.
  ```cpp
  constexpr int distance_squared(point a, point b)
  {
      return (a.x() - b.x()) * (a.x() - b.x())
           + (a.y() - b.y()) * (a.y() - b.y());
  }
  ```
  Pass-by-value is fine for tiny types like `point`; reference
  parameters would still work but aren't required.

- The trivial destructor is implicit — you don't need to write
  `~point() = default`. The default-generated dtor is trivial here
  because both members are trivially destructible (`int`).
