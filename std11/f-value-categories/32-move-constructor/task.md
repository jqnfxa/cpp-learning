# 32 — Move constructor

> Spec: <https://en.cppreference.com/cpp/language/move_constructor>

## What you learn

A **move constructor** is a constructor whose first parameter is
`T &&` (or `const T &&` / `volatile T &&`, in practice never seen).
It exists so the compiler can construct one object from the
*resources* of another when the source is about to go away.

```cpp
class buffer
{
    std::unique_ptr<int[]> data_;
    std::size_t size_;

public:
    buffer(std::size_t n) : data_(new int[n]()), size_(n) {}

    /* Move ctor: steal `other`'s buffer and length. Leave `other`
     * in a valid, empty state. */
    buffer(buffer &&other) noexcept
        : data_(std::move(other.data_))
        , size_(other.size_)
    {
        other.size_ = 0;
    }

    /* Copying makes no sense if you own a unique_ptr -- there's
     * no way to dup the pointer. So copy ops stay deleted. */
    buffer(const buffer &) = delete;
    buffer &operator=(const buffer &) = delete;
};
```

Three things to internalize:

1. **`noexcept` matters.** Without it, container types like
   `std::vector` will *copy* (instead of move) your elements on
   reallocation, because they're only allowed to move if they're
   sure no exception escapes. Always mark move ops `noexcept` when
   they really don't throw.

2. **Leave the source in a valid-but-empty state.** Not "destroyed".
   The destructor still runs on the moved-from object. Standard
   library types (`unique_ptr`, `vector`, `string`) all guarantee
   that the moved-from value is at least *destructible* and
   *assignable*. Your move ctor should do the same.

3. **Move from member to member.** When the source is `buffer
   &&other`, you call `std::move(other.data_)` on each member you
   want to move. *Inside the body*, `other.data_` looks like an
   lvalue (because it's a named subobject), so you need `std::move`
   again to keep the chain going.

## Task

In `solution.hpp`, implement `task32::buffer` with:

```cpp
class buffer
{
public:
    buffer();                       /* size 0, no allocation */
    explicit buffer(std::size_t n); /* size n, allocates n ints zero-initialized */

    buffer(buffer &&other) noexcept;          /* TODO: implement */
    /* copy ops are deleted; you don't need to write them, but the
     * delete is provided so the test for is_copy_constructible
     * works. */

    int *data();
    const int *data() const;
    std::size_t size() const;
};
```

Tests cover:
- Allocation: a buffer of size N is N ints, all 0.
- Writing through `data()` and reading back works.
- Move ctor transfers ownership: target has the data, source is
  empty (`data() == nullptr`, `size() == 0`).
- `is_copy_constructible<buffer>::value == false`.

## Run

```sh
./verify.sh
```

## Hints

- The move ctor body is the canonical "transfer + clear":
  ```cpp
  buffer::buffer(buffer &&other) noexcept
      : data_(std::move(other.data_))
      , size_(other.size_)
  {
      other.size_ = 0;
  }
  ```
- `std::unique_ptr<int[]>::operator=(unique_ptr &&)` takes care of
  itself when moved. After `std::move(other.data_)`, `other.data_
  .get() == nullptr`. The `size_` member is a plain `std::size_t`
  and won't reset itself, so you do `other.size_ = 0;` explicitly
  in the body.
- Tests use `is_copy_constructible` (false-required), so don't
  accidentally restore a copy ctor in your version.
