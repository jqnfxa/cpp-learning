# 21 — Brace-or-equal non-static data member initializers (NSDMI)

> Spec: <https://en.cppreference.com/cpp/language/data_members#Member_initialization>

## What you learn

Before C++11, you couldn't write a default value next to a class
data member's declaration. You had to write a constructor and
initialize each member there:

```cpp
struct config_old
{
    int retries;
    std::string name;
    bool verbose;

    config_old() : retries(3), name("default"), verbose(false) {}
};
```

If you added a member, you had to update every constructor's
initializer list. Easy to forget; easy to drift.

C++11 introduced **non-static data member initializers (NSDMIs)** —
defaults written right next to the declaration, using either `= value`
or `{value}`:

```cpp
struct config
{
    int retries = 3;
    std::string name{"default"};
    bool verbose = false;
};
```

Now the implicit `config()` default constructor uses these defaults.
You can still write your own constructors; their member-init lists
override the NSDMI for the members they touch:

```cpp
config c{};          /* retries=3, name="default", verbose=false */
config c2{};
c2.retries = 7;      /* retries=7, name="default", verbose=false */
```

Two forms, same effect:
- `int x = 5;` — copy-init form. Forbids `= 3.5` (no narrowing in
  list-init, but the `=` form does allow some implicit conversions
  the brace form rejects — keep reading).
- `int x{5};` — brace-init form. Strictly no narrowing.

Either is fine; `{}` is the safer default when the type is non-trivial.

## Task

In `solution.hpp`, declare `struct config` inside `task21` with these
members and defaults (using NSDMIs — *no* user-written default
constructor):

| Member         | Type           | Default     |
|----------------|----------------|-------------|
| `retries`      | `int`          | `3`         |
| `timeout_ms`   | `int`          | `5000`      |
| `name`         | `std::string`  | `"default"` |
| `verbose`      | `bool`         | `false`     |
| `tags`         | `std::vector<std::string>` | empty (default-constructed) |

`tests.cpp` checks that `config{}` produces those defaults and that
field assignments after construction work as you'd expect.

## Run

```sh
./verify.sh
```

## Hints

- Mix the two forms if you like. Convention in this codebase is
  brace-init for non-trivial types:
  ```cpp
  struct config
  {
      int retries = 3;
      int timeout_ms = 5000;
      std::string name{"default"};
      bool verbose = false;
      std::vector<std::string> tags{};
  };
  ```
  The empty `{}` on `tags` is optional — it default-constructs the
  vector, which is the same as `std::vector<std::string>()`. Spelling
  it out makes the intent visible.

- Don't write a `config()` constructor. The whole point is that the
  *implicit* default constructor honors the NSDMIs.

- Don't change any member's *type*. Tests check that, for example,
  `config{}.retries` returns an `int`-comparable value.
