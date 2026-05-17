# 15 — `std::shared_timed_mutex`

> Spec: <https://en.cppreference.com/cpp/thread/shared_timed_mutex>

## What you learn

C++14 added `std::shared_timed_mutex`: a **reader-writer
mutex** with timed operations. Two locking modes:

- **Exclusive**: only one thread holds it. Standard
  `lock()` / `unlock()`. Use for writes.
- **Shared**: multiple threads may hold it simultaneously.
  `lock_shared()` / `unlock_shared()`. Use for reads.

A writer's exclusive lock blocks all readers, and vice
versa. Multiple readers may coexist; multiple writers may
not.

```cpp
std::shared_timed_mutex mu;

void reader() {
    mu.lock_shared();
    /* read protected data */
    mu.unlock_shared();
}

void writer() {
    mu.lock();              /* exclusive */
    /* mutate protected data */
    mu.unlock();
}
```

### The "timed" in shared_timed_mutex

`shared_timed_mutex` also supports timeout-based locks:

```cpp
if (mu.try_lock_for(std::chrono::milliseconds(100))) {
    /* got the exclusive lock within 100ms */
    mu.unlock();
}

if (mu.try_lock_shared_for(std::chrono::milliseconds(100))) {
    /* got the shared lock */
    mu.unlock_shared();
}
```

Returns `true` if the lock was acquired in time, `false` on
timeout. C++17 added `std::shared_mutex` (without the
"timed" part) as a lighter alternative when timeouts aren't
needed. For C++14, `shared_timed_mutex` is the only form.

### When to reach for it

The classic "read-heavy" workload: a configuration map, a
look-up table, a cache that's queried often and updated
rarely. A regular `std::mutex` would serialize the reads,
which on a many-core machine wastes parallelism.

If reads are 95% of the workload and writes are 5%, a
shared mutex can give near-linear scaling on the read
path, where the regular mutex would be a bottleneck.

The break-even point is workload-dependent: shared mutexes
have higher per-operation overhead than plain mutexes
(maintaining the reader count, fairness, signaling). For
short critical sections with light contention, a plain
mutex can still win.

### Reader-writer fairness

The standard doesn't specify a fairness policy. Common
implementations are:

- **Reader-preferring**: a writer can starve if there's a
  continuous stream of readers.
- **Writer-preferring**: when a writer is waiting, new
  readers queue behind it.
- **Phase-fair**: alternating-batches policy.

Most real implementations are writer-preferring or
phase-fair to avoid writer starvation. If you depend on a
specific fairness guarantee, document it locally -- the
standard doesn't.

### Lock the right side

The biggest gotcha: forgetting to lock_shared on the reader
path. If you lock_exclusive on read, you've turned the
shared mutex into a plain mutex (with higher overhead).

```cpp
int get(int key) {
    mu.lock();             /* WRONG: exclusive lock on a read */
    auto v = m[key];
    mu.unlock();
    return v;
}

int get_correct(int key) {
    mu.lock_shared();      /* shared = multiple readers OK */
    auto v = m.find(key)->second;
    mu.unlock_shared();
    return v;
}
```

The compiler doesn't enforce reader-vs-writer correctness
beyond the lock-acquire/release pairing. You have to write
the right form yourself.

### Recursive locking is forbidden

`shared_timed_mutex` is **not** recursive. Locking it twice
from the same thread (in any mode) is UB:

```cpp
mu.lock_shared();
mu.lock_shared();          /* UB -- same thread already holds it shared */
```

This is the same rule as for plain `std::mutex`. If you
need recursive shared locking, that's not what this type
provides; you'd need to build your own.

### Mixing shared and exclusive

You can't upgrade a shared lock to exclusive or downgrade
exclusive to shared without releasing first:

```cpp
mu.lock_shared();
mu.lock();              /* UB -- already hold shared */

mu.unlock_shared();
mu.lock();              /* OK after releasing the shared */
```

The same goes for downgrade. C++17 added
`std::shared_mutex`, but neither version provides atomic
upgrade.

### vs std::shared_mutex (C++17)

| Type                     | Available | Has timed-lock methods |
|--------------------------|-----------|------------------------|
| `std::shared_timed_mutex`| C++14     | yes                    |
| `std::shared_mutex`      | C++17     | no                     |

If you're on C++14, you get `shared_timed_mutex` for free
(slightly more overhead than a non-timed version, but the
only choice). On C++17+, prefer `shared_mutex` for the
common case and `shared_timed_mutex` only when you need
the timed operations.

### RAII wrappers (step 16)

This step uses manual `lock_shared()` / `unlock_shared()`
to keep the focus on the mutex API. Step 16 adds
`std::shared_lock`, the RAII helper that pairs naturally
with `unique_lock` (for the exclusive side) -- the
idiomatic way to write reader-writer code.

## Task

In `solution.hpp`:

1. Change the type of `mu_` from `std::mutex` to
   `std::shared_timed_mutex`.

2. In `get()`, replace the `std::lock_guard<std::mutex>`
   with manual `mu_.lock_shared()` / `mu_.unlock_shared()`
   bookending the read.

3. In `put()`, the `std::lock_guard<...>` still does the
   right thing -- it acquires the exclusive lock. Update
   the template argument to `std::shared_timed_mutex` to
   match the new type.

```cpp
mutable std::shared_timed_mutex mu_;

int get(int key) const {
    mu_.lock_shared();
    /* ... read m_ ... */
    mu_.unlock_shared();
    return value;
}

void put(int key, int value) {
    std::lock_guard<std::shared_timed_mutex> lk(mu_);
    m_[key] = value;
}
```

## Run

```sh
./verify.sh
```

## Hints

- `<shared_mutex>` is the header. Already included in the
  scaffold.

- The mutex is declared `mutable` so it can be locked from
  `const` member functions. Standard pattern.

- The peak-concurrent-readers counter is wired up to measure
  how many threads simultaneously hold the read lock. With
  shared locking, multiple readers coexist and the peak
  rises above 1. With the stub's plain mutex, only one
  reader at a time, peak == 1.

- Each `get` holds the lock for ~20ms (artificial sleep) so
  the test has time to pile up multiple readers. In real
  code you'd hold the lock as briefly as possible.

- `lock_shared()` and `unlock_shared()` MUST be paired on the
  same thread. The scaffold's record_reader_entered /
  record_reader_left counters are independent of the lock --
  they're just instrumentation.

- The "writers_block_each_other" and "mixed_read_write_safe"
  tests pass on both stub and reference -- they test
  basic mutual exclusion, which both forms provide.

- The "concurrent_readers_overlap" test is the key
  pedagogical assertion. EXPECT_GT(peak, 1) is the contract
  shared mutexes are SUPPOSED to satisfy. If your
  implementation makes readers serialise, this test fails.

- Step 16 will replace the manual lock_shared/unlock_shared
  with `std::shared_lock<std::shared_timed_mutex>` RAII.
  Same functionality, exception-safe and easier to reason
  about.

- The C++17 alternative `std::shared_mutex` is lighter (no
  timed methods). C++14 only has the timed version.

- For library writers: `std::shared_lock` and
  `std::shared_timed_mutex` are designed to work together
  -- shared_lock can wrap either shared_mutex or
  shared_timed_mutex (or any type satisfying the SharedMutex
  concept).
