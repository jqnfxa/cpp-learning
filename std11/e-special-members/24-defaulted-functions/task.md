# 24 — `= default`

> Spec: <https://en.cppreference.com/cpp/language/function#Function_definition>

## What you learn

C++ generates several special member functions for you when it
"can" — default constructor, copy constructor, copy assignment,
destructor (and, in C++11, move constructor and move assignment). The
rules for *when* a compiler generates each are intricate and have
caused bugs for decades: declaring one special member can silently
suppress others.

C++11 added **`= default`** so you can be **explicit** about wanting
the compiler-generated version. Two scenarios make this useful:

### 1. You wrote a custom constructor and lost the default

```cpp
class widget {
public:
    widget(int id);     /* user-provided ctor */
};

widget w{};             /* error: no default constructor */
```

A user-provided constructor (the `widget(int)`) suppresses the
implicit default constructor. To get it back:

```cpp
class widget {
public:
    widget() = default;     /* explicit: I want the compiler's default ctor */
    widget(int id);
};
```

### 2. Documenting intent

Even when the compiler would have generated the member anyway,
writing `= default` makes it clear "yes, I considered this; the
default behavior is what I want". It also lets you mark the function
with `noexcept`, `constexpr`, or change its access level (public /
protected / private), separately from the default behavior.

```cpp
class widget {
public:
    widget(const widget &) = default;
    widget &operator=(const widget &) = default;
    ~widget() = default;
};
```

## Task

In `solution.hpp`, declare `task24::widget` with:

```cpp
class widget
{
public:
    widget(int id);                       /* user-provided ctor */
    widget() = default;                   /* explicit default ctor */
    widget(const widget &) = default;     /* explicit copy ctor */
    widget &operator=(const widget &) = default;  /* explicit copy assign */
    ~widget() = default;                  /* explicit dtor */

    int id() const;
private:
    int id_ = 0;
};
```

Then implement `widget(int id)` (sets `id_ = id`) and `id() const`.

The defaulted special members do *not* need any body — `= default`
is the body. The non-default `widget(int)` and the accessor are the
only functions where you write code.

`tests.cpp` pins compile-time properties (the type is
default-constructible, copy-constructible, copy-assignable) and
runtime behavior.

## Run

```sh
./verify.sh
```

## Hints

- Defaulted special members can be written *inside* the class body:
  ```cpp
  widget() = default;
  ```
  or split out below it:
  ```cpp
  widget::widget() = default;
  ```
  Either works.

- The non-defaulted `widget(int id)` and the accessor `id()` need
  real bodies. They can be inline:
  ```cpp
  widget(int id) : id_(id) {}
  int id() const { return id_; }
  ```

- The NSDMI `int id_ = 0;` provides the default value the
  defaulted constructor uses. Without it, `widget{}.id()` would be
  indeterminate.

- `static_assert(std::is_default_constructible<widget>::value, ...)`
  and friends in `tests.cpp` pin the type traits at compile time.
  If the user-provided `widget(int)` killed the default ctor and
  you didn't add the explicit `= default`, those asserts would fail.
