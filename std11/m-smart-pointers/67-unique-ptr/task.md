# 67 — `std::unique_ptr<T>`

> Spec: <https://en.cppreference.com/cpp/memory/unique_ptr>

## What you learn

`std::unique_ptr` is the C++11 replacement for the disastrous
`std::auto_ptr` (also for most uses of `new`/`delete`). It models
**exclusive ownership**: exactly one `unique_ptr` owns the
managed object, and when that `unique_ptr` dies the object is
destroyed.

The implementation is essentially:

```cpp
template <class T>
class unique_ptr
{
    T *p_ = nullptr;

public:
    unique_ptr(T *raw) : p_(raw) {}
    ~unique_ptr() { delete p_; }

    unique_ptr(unique_ptr &&) noexcept;             /* movable */
    unique_ptr(const unique_ptr &) = delete;        /* not copyable */
    unique_ptr &operator=(unique_ptr &&) noexcept;
    unique_ptr &operator=(const unique_ptr &) = delete;
    /* ...accessors get(), operator*, operator->, reset(), release()... */
};
```

The whole library facility takes one pointer-sized slot — no
reference count, no atomics, no heap of its own. Compared to
`new`/`delete`, the only thing you get is that the destructor
runs at the right time. That's exactly what you need.

### The two specialisations

```cpp
std::unique_ptr<T>      single_object;   /* calls delete  */
std::unique_ptr<T[]>    dynamic_array;   /* calls delete[] */
```

These are **different types**. Using the single-object form on a
`new T[N]` is undefined behaviour (size info lost; `delete` ≠
`delete[]`). Using the array form gives you `operator[]` and
removes `operator*`/`operator->`.

```cpp
auto a = std::unique_ptr<int[]>(new int[10]);
a[3] = 42;     /* array form: subscript */
/* *a -> error: array form has no operator* */
```

### The "no leak on early return" punchline

Before C++11, this code leaked:

```cpp
void process(int n)
{
    int *buf = new int[n];

    if (n == 0)
        return;            /* leak */
    if (validate(buf))
        throw bad_input(); /* leak */
    /* ...delete[] buf; if we forgot, leak */
}
```

The C++11 fix is one line:

```cpp
void process(int n)
{
    auto buf = std::unique_ptr<int[]>(new int[n]);

    if (n == 0)
        return;            /* buf's dtor runs -> delete[] -> no leak */
    if (validate(buf.get()))
        throw bad_input(); /* same -- stack unwinding runs buf's dtor */
    /* ... */
}
```

Every exit path runs the destructor. The leak class disappears.
This is the RAII pattern at its most fundamental: lifetime tied
to scope.

### Movable, not copyable

A `unique_ptr` can be transferred but not duplicated:

```cpp
std::unique_ptr<int[]> a(new int[5]);
auto b = std::move(a);   /* OK -- a is now empty (a.get() == nullptr) */
auto c = a;              /* compile error: copy constructor = delete */
```

This is what makes "exclusive" meaningful. If you could copy, you
would have two owners both running `delete[]` -- double free.
`auto_ptr` got this wrong (it copied with destructive semantics);
move semantics in C++11 made it possible to get it right.

## Task

In `solution.hpp`, implement three functions using
`std::unique_ptr<int[]>`:

```cpp
namespace task67
{
    /* Allocate n ints, all zero, owned by a unique_ptr<int[]>. */
    std::unique_ptr<int[]> make_zeros(std::size_t n);

    /* Sum the first n elements of arr. */
    int sum(const std::unique_ptr<int[]> &arr, std::size_t n);

    /* Allocates a unique_ptr<int[]> locally, sums it, divides by
     * `denominator`. Throws std::runtime_error("zero denominator")
     * if denominator == 0 -- the early-return / leak-free demo. */
    int safe_average(std::size_t n, int denominator);
}
```

For `make_zeros`, the cleanest C++11 spelling is
`std::unique_ptr<int[]>(new int[n]())`. The trailing `()`
value-initialises every element to 0. `std::make_unique` arrives
in C++14, so this task uses the explicit form.

## Run

```sh
./verify.sh
```

## Hints

- `unique_ptr<int[]>` provides `operator[]`. Use `arr[i]`, not
  `*arr` or `arr->`.

- Move it like any other move-only type: returning a local
  `unique_ptr` from a function is fine (NRVO + move). Don't write
  `std::move` on the return statement -- it suppresses RVO.

- For `safe_average`, do the allocation *first*, the validation
  *second*. The point is to prove the destructor fires when the
  throw unwinds the local.

- The tests can't observe a leak directly (without ASAN/LSAN),
  but they can observe that exceptions propagate cleanly and
  that subsequent allocations still succeed. If the destructor
  weren't running, you'd see memory pressure in a tight loop --
  which the tests run.

- C++14 adds `std::make_unique<T[]>(n)` to skip the `new` in the
  caller. You'll write it eventually; the C++11 idiom matters
  because that's what existed when most still-living codebases
  were written.

- A useful mental model: `unique_ptr` is what a raw pointer
  *should* have been. The fact that the raw pointer is now
  considered a code smell in modern C++ is a direct consequence
  of how rarely you need anything `unique_ptr` doesn't already
  give you.
