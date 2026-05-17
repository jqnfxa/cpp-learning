# 74 — `std::forward_list<T>`

> Spec: <https://en.cppreference.com/cpp/container/forward_list>

## What you learn

`std::forward_list<T>` is the C++11 *singly-linked* list. Each
node holds a `T` and one pointer to the next node — half the
memory overhead of `std::list` (which is doubly-linked) at the
cost of losing backward traversal and lots of operations that
need to find the previous node.

Why use it over `std::list`:

- Smaller per-node footprint (one pointer instead of two).
- No `size()`: no per-list counter, no constant-time `size()`.
  If you want size, you walk it.
- All mutators come in `_after` flavours: `insert_after`,
  `erase_after`, `emplace_after`, `splice_after`, etc. Because
  a singly-linked node knows only its successor, you operate
  by referring to the *previous* node.

```cpp
std::forward_list<int> l{1, 2, 3, 4};
l.insert_after(l.begin(), 99);   /* 1, 99, 2, 3, 4 */
```

### `before_begin()` — the trick that makes insertion at the head possible

Since you can only insert *after* a node, how do you insert at
position 0 (i.e., become the new head)? `before_begin()` returns
an iterator pointing one before the first element. It's not
dereferenceable, but you can `insert_after(before_begin(), v)`
and the new node becomes the head:

```cpp
std::forward_list<int> l{2, 3, 4};
l.insert_after(l.before_begin(), 1);   /* l == {1, 2, 3, 4} */
```

You can think of `before_begin()` as the "sentinel" position
that lets the `_after` operations cover every position uniformly,
including the head.

### Walking the list

```cpp
for (int x : l) { ... }            /* range-for */
for (auto it = l.begin(); it != l.end(); ++it) { ... }  /* iterators */
```

Iterators are *forward iterators* only: `++it` works, `--it`
doesn't. No `[]`, no `at()`, no constant-time access by index.

If you want to access element `k`, you do:

```cpp
auto it = l.before_begin();
for (std::size_t i = 0; i < k; ++i) ++it;
/* it now points to the node at position k-1; insert_after(it, v)
   inserts at position k. */
```

### When to reach for it

`forward_list` shines for memory-bounded workloads where you
have many small lists and the per-node overhead matters, or
when you only ever walk forward and use the `splice_after`
family to move chunks between lists in O(1).

For most general use, `std::vector` beats both `forward_list`
and `std::list` because cache locality dominates. Linked lists
are a niche tool in modern C++; pick them deliberately.

## Task

In `solution.hpp`, implement four functions:

```cpp
namespace task74
{
    using list = std::forward_list<int>;

    /* Insert `value` at `index`, shifting subsequent elements.
     * Precondition: 0 <= index <= length(list). */
    void insert_at(list &l, std::size_t index, int value);

    /* Erase the element at `index`.
     * Precondition: 0 <= index < length(list). */
    void erase_at(list &l, std::size_t index);

    /* Length of the list. */
    std::size_t length(const list &l);

    /* Convert to a vector for easy test assertions. */
    std::vector<int> to_vector(const list &l);
}
```

The point of the task is the *iterator gymnastics*:
`insert_at(l, 0, v)` must use `before_begin()`, and
`insert_at(l, k, v)` for `k > 0` must walk to position `k - 1`
and call `insert_after`.

## Run

```sh
./verify.sh
```

## Hints

- `before_begin()` is the key. The general pattern is:

  ```cpp
  auto prev = l.before_begin();
  for (std::size_t i = 0; i < index; ++i) ++prev;
  l.insert_after(prev, value);
  ```

  This handles `index == 0` (no advancement) and `index > 0`
  (advance `index` times so `prev` points at the node *before*
  the target).

- `erase_at` follows the same shape but ends with
  `l.erase_after(prev)`.

- `length()` walks the list and counts. There is no
  constant-time `size()` member on `forward_list` -- this is by
  design.

- `to_vector` is `std::vector<int>(l.begin(), l.end())`. The
  range constructor of `vector` does the right thing.

- `std::advance(it, n)` is the canonical way to move a forward
  iterator forward by `n` steps. For `forward_list`, `advance`
  takes O(n).

- For testing, GoogleTest's `::testing::ElementsAre` matcher
  makes the comparisons readable. We use `EXPECT_EQ` on
  vector<int> for portability — same idea, more verbose.

- `forward_list` came into C++11 partly to give the standard
  library a singly-linked option (matching what most low-level
  systems code uses) and partly to give the committee something
  on which to experiment with the `_after` family before
  considering its viability for a general singly-linked-list-
  oriented algorithm set.
