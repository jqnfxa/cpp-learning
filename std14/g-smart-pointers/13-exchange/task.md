# 13 — `std::exchange`

> Spec: <https://en.cppreference.com/cpp/utility/exchange>

## What you learn

`std::exchange(obj, new_value)` writes `new_value` to `obj`
and returns the **old** value. It's a one-liner that
replaces the common "save the old, then overwrite" pattern:

```cpp
/* Before std::exchange */
T old = obj;
obj = new_value;
return old;

/* With std::exchange */
return std::exchange(obj, new_value);
```

One expression, no temporary name, atomic-feeling sequence.
The function lives in `<utility>` and is `constexpr` in C++14+.

### The signature

```cpp
template <class T, class U = T>
T exchange(T &obj, U &&new_value);
```

- Takes a reference to the target.
- Takes any value convertible to `T`.
- Returns the **old** value of `obj` (by value).

### The canonical use: move ctor / move assignment

The C++14 textbook example. A class owning a raw resource
needs to zero out the source on move:

```cpp
class buffer
{
    int *data_;
    std::size_t size_;
public:
    buffer(buffer &&other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0))
    {}

    buffer &operator=(buffer &&other) noexcept
    {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }
};
```

Without `std::exchange`, you'd write:

```cpp
buffer(buffer &&other) noexcept
    : data_(other.data_), size_(other.size_)
{
    other.data_ = nullptr;
    other.size_ = 0;
}
```

That works too, but it has two failure modes:
1. **Forgetting** a field (the scaffold's stub does this).
2. **Order-of-init**: with member initializers, the body
   runs AFTER both members are constructed. Fine for
   `nullptr` and `0`, but if a future maintainer adds a
   member with a side-effecting move, you can get subtle
   bugs.

`exchange` localizes the "take then null" operation to a
single expression -- harder to forget, harder to misorder.

### Pop / swap idioms

`std::exchange` is also the cleanest way to write "return
the old value and reset":

```cpp
class counter {
    int value_;
public:
    int pop() { return std::exchange(value_, 0); }       /* reset */
    int swap(int v) { return std::exchange(value_, v); } /* replace */
};
```

This is a common pattern in queue-style APIs (`take()`,
`steal()`) and in pull-style state machines.

### Why is the return type just `T` (by value)?

Because the OLD value is being copied / moved out before the
new value is written. The signature is essentially:

```cpp
template <class T, class U = T>
T exchange(T &obj, U &&new_value)
{
    T old = std::move(obj);              /* move-construct old */
    obj = std::forward<U>(new_value);    /* perfect-forward new */
    return old;                          /* NRVO / move */
}
```

So `obj` is left in the moved-from-then-reassigned state
(typically equivalent to `new_value`), and the caller gets
the previous value.

### How it differs from `std::swap`

`std::swap(a, b)` exchanges TWO values; both `a` and `b`
end up with the OTHER's previous value:

```cpp
std::swap(a, b);    /* a was X, b was Y; now a is Y, b is X */
```

`std::exchange(a, b)` returns `a`'s previous value and sets
`a = b`. `b` itself is unchanged (the new value is
forwarded into `a`, not into `b`):

```cpp
int old = std::exchange(a, b);    /* old == X; a == Y; b is unchanged */
```

When you want "save and overwrite," it's `exchange`. When
you want "trade two values," it's `swap`.

### constexpr (C++17+ for std::exchange)

In C++14, `std::exchange` was not `constexpr`. C++17 made it
`constexpr`. For our C++14 code, this means it works at
runtime but not in a constant expression.

If you're writing C++14 and need exchange-like behavior in a
constant expression, you'd write:

```cpp
template <class T, class U>
constexpr T cx_exchange(T &obj, U &&v)
{
    T old = obj;
    obj = std::forward<U>(v);
    return old;
}
```

For runtime code, just use `std::exchange`.

### Where you'll see it in real code

- Move constructors / move assignment of resource-owning
  classes.
- Pull operations: take the value off a state, leave the
  default.
- Atomic-emulation code: "claim ownership, replace with
  null."
- Functional helpers: combining a "take" and a "store" into
  one statement.

It's a small utility but disproportionately common once you
learn the idiom.

## Task

In `solution.hpp`, fill in four function bodies to use
`std::exchange`:

```cpp
/* counter::pop */
int pop() noexcept { return std::exchange(value_, 0); }

/* counter::swap */
int swap(int new_value) noexcept {
    return std::exchange(value_, new_value);
}

/* buffer::buffer(buffer&&) */
buffer(buffer &&other) noexcept
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
{}

/* buffer::operator=(buffer&&) */
buffer &operator=(buffer &&other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
    }
    return *this;
}
```

## Run

```sh
./verify.sh
```

## Hints

- `std::exchange` is in `<utility>`. The header is already
  included in the scaffold.

- For `counter::pop`, the new value is `0` (reset to default).

- For `counter::swap`, the new value is the parameter
  `new_value`.

- For `buffer`'s move members, use `std::exchange` for
  BOTH `data_` and `size_`. The stub correctly nulls
  `data_` but forgets `size_` -- the "zeros_source_size"
  tests catch this.

- Don't add `if (this != &other)` to the move CTOR -- it's
  legal but redundant (a fresh object can't alias itself).
  The move ASSIGNMENT does need the self-check because
  `a = std::move(a)` is technically allowed.

- The destructor stays as-is (`delete[] data_`). After move
  the source's `data_` is null, and `delete[] nullptr` is a
  no-op -- no double-free.

- Tests verify: counter pop/swap return-and-modify behavior;
  buffer's move-ctor transfers data + nulls source data +
  zeros source size; same for move-assign; destructor is
  safe after a move (no double-free).

- C++17 made `std::exchange` constexpr. C++14's version is
  runtime-only. None of our tests need constexpr-exchange.
