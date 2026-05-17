# 69 — `std::shared_ptr<T>`

> Spec: <https://en.cppreference.com/cpp/memory/shared_ptr>

## What you learn

Where `unique_ptr` models *exclusive* ownership,
`std::shared_ptr` models *shared* ownership. Multiple
`shared_ptr` instances can refer to the same managed object;
the object is destroyed when the last one is destroyed.

The bookkeeping lives in a small heap-allocated *control block*
shared by all the `shared_ptr`s for one object. The control
block holds:

- A **strong count** (`use_count`) — how many `shared_ptr`s
  currently own the object. Hits zero -> object destroyed.
- A **weak count** — how many `weak_ptr`s (and the strong
  count's own "1 if non-zero" entry) still need the control
  block to stick around. Hits zero -> control block destroyed.
- A type-erased deleter (default: `delete p`).
- Often the allocator and the object itself (when constructed
  via `std::make_shared`).

```cpp
std::shared_ptr<int> a(new int(42));   /* a.use_count() == 1 */
auto b = a;                             /* a.use_count() == 2 */
{
    auto c = a;                         /* a.use_count() == 3 */
}                                        /* a.use_count() == 2 */
a.reset();                              /* a.use_count() == 0; b.use_count() == 1 */
```

### The right way to construct: `make_shared`

```cpp
auto p = std::make_shared<int>(42);
```

`make_shared` allocates the object and the control block in a
**single allocation**. This is faster than `shared_ptr<T>(new
T(...))` (one alloc vs two), avoids a sneaky exception-safety
trap with the two-argument form, and lets the control block be
freed alongside the object's memory.

A small trade-off: with `make_shared`, the object can only be
freed after the last `weak_ptr` is gone too (because the
allocation is shared). For most uses that's fine; it matters
only if `T` is large and you cache `weak_ptr`s for a long time.

(Step 70 covers `make_shared` in depth; this step uses it.)

### Reference counting is **atomic**

The control block's counters are atomic. That means:

- You can share a `shared_ptr<T>` *the smart pointer* across
  threads safely — copying, destroying, and `reset()` all
  manipulate the count atomically.
- You **cannot** share the same `shared_ptr<T>` *instance*
  (i.e. the variable itself) across threads without external
  synchronisation — that's still a data race on the variable.
- And the *object* `T` is unaffected: `shared_ptr` doesn't make
  `T`'s operations atomic. If two threads write to `*p`, that's
  a race in `T`, not in the smart pointer.

The atomic counts are not free. `shared_ptr` is more expensive
than `unique_ptr` for that reason; reach for it only when you
*need* shared ownership.

### `use_count()`

`p.use_count()` returns the number of `shared_ptr` instances
that share ownership of `*p`. It's `long`-typed in the
standard. Use it for diagnostics; don't make program decisions
based on the precise value in production code, because in
multi-threaded contexts it's a snapshot that can change.

## Task

In `solution.hpp`, implement an API that demonstrates sharing
ownership between two functions and exposes `use_count()`:

```cpp
namespace task69
{
    /* Allocate an int(n) and return a shared_ptr owning it. */
    std::shared_ptr<int> make_value(int n);

    /* Stash a *copy* of p in a function-static slot. Returns the
     * stashed copy's use_count() after the assignment. */
    long stash(const std::shared_ptr<int> &p);

    /* Drop the stashed copy. Returns true if something was held,
     * false if the stash was already empty. */
    bool release();

    /* Return a copy of whatever is currently stashed (may be empty). */
    std::shared_ptr<int> stashed();
}
```

The "two functions sharing" idea: tests build a local
`shared_ptr`, hand it to `stash`, and then observe that
`stash`'s function-static and the local point to the same
object — visible as `use_count()` going from 1 to 2 (and 3 etc.
when more copies are pulled out).

## Run

```sh
./verify.sh
```

## Hints

- Use `std::make_shared<int>(n)` inside `make_value`. The whole
  point of this step is to use the modern construction idiom.

- The function-static slot inside `stash` should be a
  *function-local* `static std::shared_ptr<int>` — the
  function-local static gives you per-process state without
  exposing a global. Returning its `use_count()` after assignment
  is what the tests pin.

- `stash` must take its argument by **const reference**. Taking
  by value would increment the count an extra time during the
  call, giving misleading numbers.

- `release()` returns true if the static slot was non-empty
  before the reset. Use `static_cast<bool>(slot)` or just `slot
  != nullptr` to check.

- `stashed()` returns by value (a `shared_ptr<int>` copy) — that
  copy's *own* `use_count()` will of course include itself. The
  tests account for this.

- This is one of those rare APIs where the function-local static
  is the right tool. In a real codebase it should be a
  dependency-injected slot, not a global stash; here it's a
  vehicle for observing reference-count behaviour without
  cluttering the test code.
