# 70 — `std::make_shared`

> Spec: <https://en.cppreference.com/cpp/memory/make_shared>

## What you learn

`std::shared_ptr` has a small dirty secret: when you construct it
from a raw pointer, the implementation has to allocate a
**second** chunk of memory for the control block. So:

```cpp
auto p = std::shared_ptr<T>(new T(args...));
        /* ----- alloc #1 ----- */
        /* alloc #2 happens inside the shared_ptr constructor: */
        /*       the control block (refcount + deleter) */
```

`std::make_shared<T>(args...)` is the C++11 fix:

```cpp
auto p = std::make_shared<T>(args...);
        /* ONE allocation: object + control block in one block */
```

The single allocation is the headline. Two more wins fall out of
it:

1. **Better cache locality.** The control block and the object
   are adjacent in memory; ref-count manipulation and object
   access touch the same cache line.

2. **Exception safety.** A pre-C++17 trap with the two-arg form:

   ```cpp
   foo(std::shared_ptr<T>(new T()), bar());
   ```

   The compiler is allowed to evaluate `new T()` and then call
   `bar()` and *then* hand the pointer to the `shared_ptr`
   constructor. If `bar()` throws between `new` and the
   `shared_ptr` taking ownership, the `T` leaks.
   `make_shared<T>()` collapses construction into one indivisible
   call -- no exposed raw pointer, no leak.
   (C++17 tightened the evaluation rules; the trap is mostly
   historical, but still listed in Effective Modern C++ Item 21.)

### The one trade-off

`make_shared` puts the object *inside* the control block's
allocation. The whole block is freed only after the last
`shared_ptr` *and* the last `weak_ptr` are gone (the control
block has to outlive every reference to it). With the two-arg
form, the object can be freed independently of the control block:
when strong count hits zero the object is destroyed and its
memory is freed; the control block sticks around for weak refs.

In practice this matters only when:
- `T` is large, *and*
- you keep `weak_ptr`s around well after the last `shared_ptr`
  is gone.

When that's a real concern, fall back to the two-arg form. For
everything else: `make_shared`.

### Forwarding the constructor arguments

`make_shared<T>(args...)` forwards `args` to `T`'s constructor
via *perfect forwarding* (uses `std::forward`). That means:

```cpp
struct Point { Point(int, int); };

auto p = std::make_shared<Point>(3, 4);   /* calls Point(3, 4) */
```

works the same as `Point(3, 4)`, including rvalue arguments,
const correctness, and braced-init forwarding caveats. (The
braced-init caveat: `make_shared<Vec>{1, 2, 3}` does not work
because templates can't deduce an initializer-list argument from
braces — you'd need `make_shared<Vec>(std::initializer_list<int>{1,2,3})`,
or just plain `Vec{1,2,3}` wrapped after the fact.)

## Task

In `solution.hpp`, implement three small factories so the tests
can compare the two construction idioms head-to-head:

```cpp
namespace task70
{
    struct Point
    {
        int x;
        int y;
        Point(int xx, int yy) : x(xx), y(yy) {}
    };

    /* The old idiom: two allocations. Implement with new + raw pointer. */
    std::shared_ptr<int> make_via_new(int n);

    /* The modern idiom: one allocation. Implement with make_shared. */
    std::shared_ptr<int> make_via_make_shared(int n);

    /* Forward both arguments to Point's constructor via make_shared. */
    std::shared_ptr<Point> make_point(int x, int y);
}
```

The tests count global heap allocations across each call and
assert:

- `make_via_new` performs **2** allocations.
- `make_via_make_shared` performs **1** allocation.
- `make_point` performs **1** allocation and constructs `Point`
  with the right `x` and `y`.

The allocation count test is the proof of what task.md claims —
it's why this step exists as a separate task from step 69.

## Run

```sh
./verify.sh
```

## Hints

- `make_via_new` is one line:
  `return std::shared_ptr<int>(new int(n));`. Yes, this is the
  bad idiom — the point is to make the difference visible.

- `make_via_make_shared` is one line:
  `return std::make_shared<int>(n);`.

- `make_point` is `std::make_shared<Point>(x, y);` — the two
  args are forwarded to `Point(int, int)`.

- The tests count *global* `operator new` calls by installing
  a replacement. Don't worry about how the counting works; your
  job is just to use the right factories on the solution side.

- Do not call `make_shared` for `make_via_new` "to be safer".
  The point of having both factories is to *measure* the
  difference. Implement each one literally as named.

- A very common interview question: "Why prefer `make_shared`?"
  Three answers: one alloc not two, cache locality, exception
  safety. The one downside (object lifetime tied to control
  block lifetime) is the follow-up.
