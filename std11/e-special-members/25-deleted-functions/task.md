# 25 — `= delete`

> Spec: <https://en.cppreference.com/cpp/language/function#Function_definition>

## What you learn

`= delete` is the dual of `= default`. Where `= default` says
"please generate the standard implementation", `= delete` says
**"this function does not exist; the compiler must reject any
attempt to call it"**.

The classic case is making a type non-copyable while still movable:

```cpp
class unique_resource
{
public:
    unique_resource() = default;
    unique_resource(unique_resource &&) = default;
    unique_resource &operator=(unique_resource &&) = default;

    unique_resource(const unique_resource &) = delete;
    unique_resource &operator=(const unique_resource &) = delete;
};

unique_resource a;
unique_resource b{a};         /* error: deleted function */
unique_resource c{std::move(a)};   /* OK: moves */
```

Pre-C++11, you'd accomplish this with a private declaration and no
definition. Two problems with that idiom:
- Inside the class itself or a friend, you could still call the
  function (you'd hit a link error, not a compile error).
- The error message ("undefined reference to copy ctor at link time")
  was misleading.

`= delete` produces a clear compile-time error at the call site
("call to deleted function"). And it works on *any* function, not
just special members — you can delete a particular overload to
forbid specific conversions.

```cpp
void f(int);
void f(double) = delete;     /* explicitly forbid the float overload */

f(3);     /* OK: f(int) */
f(3.5);   /* error: f(double) is deleted */
```

## Task

In `solution.hpp`, declare `task25::unique_resource` with:

```cpp
class unique_resource
{
public:
    unique_resource() = default;

    unique_resource(unique_resource &&) = default;
    unique_resource &operator=(unique_resource &&) = default;

    unique_resource(const unique_resource &) = delete;
    unique_resource &operator=(const unique_resource &) = delete;

    int id() const;
private:
    int id_ = 7;
};
```

(The exact value `7` doesn't matter; it gives the move ops something
to verify.)

Tests pin:
- `is_default_constructible<unique_resource>` is `true`.
- `is_copy_constructible<unique_resource>` is `false`.
- `is_copy_assignable<unique_resource>` is `false`.
- `is_move_constructible<unique_resource>` is `true`.
- `is_move_assignable<unique_resource>` is `true`.

And at runtime that an instance is default-constructible, returns
`id() == 7`, and can be move-constructed.

## Run

```sh
./verify.sh
```

## Hints

- The five lines you write inside the class body are exactly the
  five lines in the table — three `= default`, two `= delete`.

- `= delete` works on any function. It doesn't have to follow `=
  default`. You can delete a function the compiler wouldn't have
  generated anyway, just to forbid a specific call shape:
  ```cpp
  class id_holder
  {
  public:
      explicit id_holder(int);
      explicit id_holder(double) = delete;  /* no implicit float -> int */
  };
  ```

- In tests.cpp you'll see things like
  `static_assert(!std::is_copy_constructible<unique_resource>::value, ...)`.
  That trait reads the deleted copy constructor and reports `false`.
  Pre-C++11, the private-undefined idiom didn't surface to traits
  this cleanly.
