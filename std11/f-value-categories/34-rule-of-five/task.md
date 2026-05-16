# 34 — Rule of five

> Spec: <https://en.cppreference.com/cpp/language/rule_of_three>

## What you learn

"Rule of three" (pre-C++11): if your class needs a user-defined
**destructor**, **copy constructor**, *or* **copy assignment
operator** for correctness, it almost certainly needs all three.
The classic case is owning a raw resource (`new`, `malloc`, a file
handle, a socket): the implicit memberwise copy would alias the
resource between two objects, and the destructor would `delete`
it twice.

C++11 added move operations, so the rule grew to **five**: if you
write one of {`~T()`, `T(const T &)`, `T &operator=(const T &)`,
`T(T &&)`, `T &operator=(T &&)`}, you almost certainly need all
five.

Some declarations interact in subtle ways:

| If you write...           | The compiler implicitly...                              |
|---------------------------|---------------------------------------------------------|
| `~T()`                    | *Deprecates* implicit copy. (Still generates it.)       |
| `T(const T &)` or `=`     | Suppresses move ctor and move assign.                   |
| `T(T &&)` or `=`          | Suppresses copy ctor (deleted), copy assign (deleted).  |

So if you write *just* a destructor, you'll get implicit copy (with
the wrong semantics for an owning class) but *no* move. Writing the
move ops without the copy ops makes the type move-only.

**Rule of zero** is the modern preferred form: don't own raw
resources yourself. Use `std::unique_ptr`, `std::vector`, etc., as
members; their special-member functions compose correctly and you
write nothing. The C++ Core Guidelines recommend rule of zero for
new code and rule of five when you do own a raw resource.

## Task

`solution.hpp` provides a `task34::handle` class with a destructor
already written (it `delete`s an owned `int *`). The default ctor
and the value ctor are provided. Your job: write the **other four**
special members so the type satisfies the rule of five:

```cpp
class handle
{
public:
    handle() noexcept;                  /* provided: data_ = nullptr */
    explicit handle(int v);             /* provided: data_ = new int(v) */
    ~handle();                          /* provided: delete data_ */

    handle(const handle &other);                   /* TODO: deep copy */
    handle &operator=(const handle &other);        /* TODO: deep copy with self-assign guard */
    handle(handle &&other) noexcept;               /* TODO: steal pointer, null source */
    handle &operator=(handle &&other) noexcept;    /* TODO: steal pointer with self-move guard */

    bool has_value() const;
    int value() const;
};
```

Semantics:

| State                  | `has_value()` | `value()`                         |
|------------------------|---------------|-----------------------------------|
| Default constructed    | `false`       | UB                                |
| `handle{42}`           | `true`        | `42`                              |
| After copy ctor        | `true` (independent allocation) | original's value |
| After move ctor        | original goes to `has_value()==false` | new owns the original ptr |

Tests:
- 8 runtime tests covering each special member.
- Compile-time checks that the type is `copy_constructible`,
  `copy_assignable`, `move_constructible`, `move_assignable`, all
  of them `nothrow_destructible`.

## Run

```sh
./verify.sh
```

## Hints

- For copy ctor, the source might be empty:
  ```cpp
  handle::handle(const handle &other)
      : data_(other.data_ ? new int(*other.data_) : nullptr)
  {
  }
  ```

- Copy assign with self-check:
  ```cpp
  handle &handle::operator=(const handle &other)
  {
      if (this != &other)
      {
          int *new_data = other.data_ ? new int(*other.data_) : nullptr;
          delete data_;
          data_ = new_data;
      }
      return *this;
  }
  ```
  *Allocate before delete* so the assignment is strong-exception-safe.

- Move ctor: steal and null:
  ```cpp
  handle::handle(handle &&other) noexcept
      : data_(other.data_)
  {
      other.data_ = nullptr;
  }
  ```

- Move assign with self-check:
  ```cpp
  handle &handle::operator=(handle &&other) noexcept
  {
      if (this != &other)
      {
          delete data_;
          data_ = other.data_;
          other.data_ = nullptr;
      }
      return *this;
  }
  ```

- Mark move ops `noexcept` so containers will use them on
  reallocation (step 35's topic).
