# 22 — Delegating constructors

> Spec: <https://en.cppreference.com/cpp/language/constructor#Delegating_constructor>

## What you learn

A C++11 **delegating constructor** lets one constructor call another
in its member-initializer list:

```cpp
class point
{
    int x_;
    int y_;
public:
    point() : point(0, 0) {}            /* delegates to (int, int) */
    point(int v) : point(v, v) {}       /* delegates to (int, int) */
    point(int x, int y) : x_(x), y_(y) {}  /* the "target" */
};
```

Pre-C++11, you'd either duplicate the initialization logic across
constructors or move it into a private `init()` member function.
Delegation makes the relationship between constructors explicit.

The rules:

1. **Either delegate or initialize, not both.** A delegating
   ctor's init list contains *exactly one* item: the delegation call.
   You can't write `point(int v) : point(v, v), some_other_(0) {}`.
2. **No cycles.** `A() : A(1) {}` and `A(int) : A() {}` is UB at
   runtime; the compiler usually warns.
3. **The target ctor runs first.** The body of the delegating ctor
   runs only after the target ctor has finished and the object is
   fully constructed.

## Task

In `solution.hpp`, declare `task22::point` with three constructors
and two read-only accessors:

```cpp
class point
{
    int x_;
    int y_;
public:
    point();                     /* delegate to point(0, 0)   */
    point(int v);                /* delegate to point(v, v)   */
    point(int x, int y);         /* the target; initializes x_ and y_ */

    int x() const;
    int y() const;
};
```

| Constructor          | Result          |
|----------------------|-----------------|
| `point()`            | `x=0, y=0`      |
| `point(5)`           | `x=5, y=5`      |
| `point(3, 4)`        | `x=3, y=4`      |

Both delegating constructors **must** delegate via the init list —
not by calling each other from the body, and not by duplicating the
`x_(...)` / `y_(...)` lines.

## Run

```sh
./verify.sh
```

## Hints

- Member-init-list delegation syntax:
  ```cpp
  point::point() : point(0, 0) {}
  ```
  Note: it's `point(...)`, the class's *own* type, not
  `point_target` or anything renamed.

- The accessors can be one-liners:
  ```cpp
  int point::x() const { return x_; }
  int point::y() const { return y_; }
  ```

- You can write everything in the class body (inline) or split the
  definitions out below it. Either is fine.

- The tests use `EXPECT_EQ(p.x(), ...)` so the accessors must return
  the values stored.
