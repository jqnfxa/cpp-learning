# 129 — Custom allocator (minimal C++11 form)

> Spec: <https://en.cppreference.com/cpp/memory/allocator>

## What you learn

A C++11 **allocator** is a small handle type that knows how to
get raw memory from somewhere and give it back. STL containers
take an allocator as their second template parameter and
delegate every storage decision through it.

A custom allocator lets you:
- Count / log allocations (this step's exercise).
- Pool small fixed-size blocks.
- Pin memory in a specific NUMA region or huge-page arena.
- Use a stack/arena instead of the heap.

C++11 reduced the allocator requirements to just a few
members; the rest are filled in by `std::allocator_traits`.

### The minimum a C++11 allocator needs

```cpp
template <typename T>
struct my_alloc
{
    using value_type = T;

    my_alloc() noexcept {}

    template <typename U>
    my_alloc(const my_alloc<U>&) noexcept {}   /* rebind ctor */

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n) noexcept;
};

template <typename A, typename B>
bool operator==(const my_alloc<A>&, const my_alloc<B>&) noexcept;

template <typename A, typename B>
bool operator!=(const my_alloc<A>&, const my_alloc<B>&) noexcept;
```

That's it -- no `pointer`, no `size_type`, no `rebind`, no
`construct`/`destroy`/`max_size`/`address`. C++11
`std::allocator_traits<my_alloc<T>>` synthesises everything
else from `value_type` plus the two member functions.

### Why the rebind ctor

A `std::vector<int, my_alloc<int>>` internally also needs
allocators of *other* types (e.g. internal nodes). It does so
via `allocator_traits::rebind_alloc<U>`, which uses the
template ctor:

```cpp
template <typename U>
my_alloc(const my_alloc<U>&) noexcept {}
```

This converts a `my_alloc<int>` into a `my_alloc<U>` for any
`U`. For stateless allocators it's a no-op; for stateful ones
the state (an arena pointer, etc.) is copied across.

### Why the equality operators

Two allocators are "equal" if memory allocated by one can be
freed by the other. The container uses `==` / `!=` to decide
whether it can move-construct from another container's storage
or must copy. For a stateless `operator new`-backed allocator,
all instances are interchangeable -- `==` returns `true`.

### Counting allocator

The exercise: bump global counters on each `allocate` and
`deallocate`. The container does the rest -- you just observe.

```cpp
struct alloc_stats {
    std::size_t allocations;
    std::size_t deallocations;
    std::size_t bytes_allocated;
};

inline alloc_stats& stats() {
    static alloc_stats s = {0, 0, 0};
    return s;
}

template <typename T>
struct counting_allocator
{
    using value_type = T;

    counting_allocator() noexcept {}
    template <typename U>
    counting_allocator(const counting_allocator<U>&) noexcept {}

    T* allocate(std::size_t n)
    {
        ++stats().allocations;
        stats().bytes_allocated += n * sizeof(T);
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) noexcept
    {
        ++stats().deallocations;
        ::operator delete(p);
    }
};
```

The function-static `stats()` pattern works around the C++11
"non-`constexpr` static data members need an out-of-class
definition" rule. (C++17 introduced inline variables, which
make `static inline` data members single-definition. C++11
doesn't have that, so we use a function returning a static
local instead.)

### Plugging it into std::vector

```cpp
using ivec = std::vector<int, counting_allocator<int>>;

ivec v;
v.push_back(1);    /* allocate(1)     -- first reservation */
v.push_back(2);    /* allocate(2),    -- grow + deallocate(1) */
v.push_back(3);    /* (within capacity) */
```

After the vector goes out of scope, `deallocations ==
allocations` -- every block has been returned.

### What about `construct` / `destroy`?

In C++03 you had to implement `construct(p, val)` and
`destroy(p)`. C++11 made these optional: if you don't provide
them, `allocator_traits::construct` falls back to placement
new (`::new (p) T(args...)`), which is what you'd write anyway.
You only need to override them if you want to track
construction explicitly or do something exotic.

### Stateful allocators

The interesting case is stateful -- an arena allocator carries
a pointer to a `char[]` slab and `allocate(n)` bumps a marker.
For those, the rebind ctor must copy the pointer, and
`operator==` should compare the arena pointers (not return
`true` blindly). Containers also need `propagate_on_*` traits
to know whether to move/copy the allocator on
move/copy-assign. That's the next step (`scoped_allocator_adaptor`).

## Task

In `solution.hpp`, fill in `counting_allocator<T>::allocate` and
`counting_allocator<T>::deallocate` so they bump the counters
in `stats()` before doing the actual allocation.

```cpp
namespace task129
{
    struct alloc_stats {
        std::size_t allocations;
        std::size_t deallocations;
        std::size_t bytes_allocated;
    };
    inline alloc_stats& stats();

    template <typename T>
    struct counting_allocator {
        using value_type = T;
        T* allocate(std::size_t n);
        void deallocate(T* p, std::size_t n) noexcept;
    };

    using ivec = std::vector<int, counting_allocator<int>>;
}
```

## Run

```sh
./verify.sh
```

## Hints

- The TODOs are inside `allocate` and `deallocate` in the scaffold.

- `allocate` increments `stats().allocations` once and adds
  `n * sizeof(T)` to `stats().bytes_allocated`.

- `deallocate` increments `stats().deallocations`.

- Don't touch the existing `::operator new` / `::operator delete`
  calls -- those do the actual work.

- The scaffold throws `std::bad_alloc` on overflow. Keep that
  guard; it's the standard contract.

- Multiple `std::vector` reallocations within one push-back loop
  are normal: vector grows geometrically (~1.5x or 2x), each
  growth = one allocate + one deallocate of the OLD buffer.

- `EXPECT_GE` is used in many tests because the precise number
  of reallocations depends on the standard library's growth
  policy. The contract is "at least one", not "exactly N".

- After a `vector` goes out of scope, `allocations ==
  deallocations`: every block returned. This test catches "I
  forgot to bump the deallocation counter" *and* "I bumped it
  twice".

- `stats()` returns a *reference* to a function-local static.
  This is C++11's only ergonomic way to give a header-only
  module a shared mutable global (without C++17 `inline`
  variables, without an out-of-class definition in a `.cpp`).

- Stateless allocators of *related* types must compare equal:
  `counting_allocator<int>{} == counting_allocator<double>{}`.
  This lets `vector<int>` happily steal a buffer if the
  internals rebind to a different element type.

- Tests verify: `value_type` alias, direct alloc/dealloc
  counter bumps, vector's empty-state non-allocation,
  push_back triggers alloc, growth triggers multiple, dtor
  triggers dealloc, alloc/dealloc balance, byte tracking,
  value correctness, equality across rebind, shared stats
  across multiple vectors.
