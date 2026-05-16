# 31 — `std::move`

> Spec: <https://en.cppreference.com/cpp/utility/move>

## What you learn

`std::move(x)` is the bridge from step 30's gotcha. The named
rvalue-reference `rr` is an lvalue when used; `std::move(rr)`
converts the expression to an *xvalue* so that overload resolution
picks the rvalue overload.

The implementation is one cast:

```cpp
template <class T>
constexpr typename std::remove_reference<T>::type &&
move(T &&t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type &&>(t);
}
```

That is: `std::move` **does not move anything**. It just changes the
*value category* of the expression so that a moving constructor or
moving assignment operator is selected at the next step. Think of
it as `static_cast<T &&>(...)` with a more readable name.

## The canonical swap

The textbook use of move semantics is `swap`. Pre-C++11, the
"copy + assignment" `swap` made up to three full deep copies for a
big object:

```cpp
template <class T>
void swap(T &a, T &b)
{
    T tmp = a;     /* deep copy */
    a = b;         /* deep copy */
    b = tmp;       /* deep copy */
}
```

With `std::move`, those three "copies" become three moves — for any
type that has a move constructor and move assignment, the actual
storage gets pointer-swapped instead of being deep-copied.

```cpp
template <class T>
void swap(T &a, T &b)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}
```

For `std::string`, `std::vector`, `std::unique_ptr` — anything that
owns a heap-allocated resource — this is *enormously* faster on
average.

## Task

In `solution.hpp`, implement:

```cpp
template <class T>
void swap(T &a, T &b);
```

The body is three lines: declare a `tmp` initialized from
`std::move(a)`, assign `std::move(b)` to `a`, assign
`std::move(tmp)` to `b`. No other temporaries.

Tests cover:
- `int` swap (correctness on a trivially-copyable type).
- `std::string` swap (the meaningful case for move).
- `std::unique_ptr<int>` swap (a move-only type — would fail to
  compile if the student wrote a copy-based swap).
- Self-swap is well-defined (swap of `x` with `x` leaves it
  unchanged).

## Run

```sh
./verify.sh
```

## Hints

- The implementation is:
  ```cpp
  T tmp = std::move(a);
  a = std::move(b);
  b = std::move(tmp);
  ```
  No extra braces, no `auto` deductions, exactly three statements.

- `std::move` is in `<utility>`. Include it.

- Why does the `std::unique_ptr` test demand move semantics?
  `unique_ptr` deletes its copy constructor (step 25). If your
  `swap` did `T tmp = a;` (copy), `tmp = unique_ptr<int>(a)` would
  fail to compile because the copy ctor is deleted. The `std::move`
  cast routes the call to the move ctor, which `unique_ptr` does
  have.

- Self-swap with this idiom: `swap(x, x)` is safe and a no-op.
  `tmp = move(x)` leaves `x` in a valid-but-unspecified state, but
  then `x = move(x)` moves it back, and so on. For `int` this is
  trivially correct; for class types you need to make sure your
  move-assignment handles self-assignment, which the standard
  containers all do.
