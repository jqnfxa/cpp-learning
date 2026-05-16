# 33 — Move assignment operator

> Spec: <https://en.cppreference.com/cpp/language/move_assignment>

## What you learn

A **move assignment operator** is the partner of the move
constructor (step 32). Where the move ctor builds *a new object*
from a soon-to-die source, the move-assignment operator replaces
*an existing object*'s state with the source's state.

Signature:

```cpp
T &operator=(T &&other) noexcept;
```

Three structural differences from the move ctor:

1. **The target already exists.** You have to release/replace
   whatever the target owns *before* (or as part of) stealing the
   source's resources.
2. **Self-move is possible.** `a = std::move(a)` is a real
   expression. Without a guard, your move-assign could destroy
   `a`'s data while trying to move from `a` to `a`.
3. **It returns `*this`.** Like all assignment operators in C++,
   for chaining (`a = b = c;` and so on).

The canonical, defensive form:

```cpp
buffer &operator=(buffer &&other) noexcept
{
    if (this != &other)
    {
        data_ = std::move(other.data_);   /* unique_ptr's move-assign releases the old data and steals the new */
        size_ = other.size_;
        other.size_ = 0;
    }
    return *this;
}
```

The self-check `if (this != &other)` is the cheap-and-correct guard.
Some implementations skip it (relying on individual member move-
assigns to be self-safe, which the standard library's are), but
*adding* the check is harmless and makes intent explicit.

## Task

`buffer` from step 32 is provided again. Add the move-assignment
operator. The class's move ctor is already implemented, so you can
focus on assignment.

```cpp
buffer &operator=(buffer &&other) noexcept;
```

| Operation                                | Expected state                |
|------------------------------------------|-------------------------------|
| `a = std::move(b)` (a, b different)      | a steals b's data; b empty    |
| `a = std::move(a)` (self-move)           | a unchanged (defensive)       |

Tests cover:
- Compile-time `is_move_assignable`, `is_nothrow_move_assignable`.
- Basic move-assign transfers ownership and empties the source.
- Self-move-assign leaves the buffer valid (we test for "size and
  data still match what they were").
- Move-assigning to a buffer that already held data correctly
  releases the old data.

## Run

```sh
./verify.sh
```

## Hints

- The body (with self-check):
  ```cpp
  if (this != &other)
  {
      data_ = std::move(other.data_);
      size_ = other.size_;
      other.size_ = 0;
  }
  return *this;
  ```
- `data_ = std::move(other.data_)` calls `unique_ptr::operator=`'s
  move overload, which releases the old `data_` and steals
  `other`'s. You don't need to write any `delete[]` — that's the
  whole point of using `unique_ptr`.
- The function returns `*this` (by reference) so you can chain
  assignments and so the call expression has a sensible type.
