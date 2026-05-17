# 16 — `std::shared_lock`

> Spec: <https://en.cppreference.com/cpp/thread/shared_lock>

## What you learn

`std::shared_lock<Mutex>` is the RAII helper that pairs with
`std::shared_timed_mutex` (or C++17's `std::shared_mutex`)
for the SHARED side. It's the reader-side counterpart of
`std::unique_lock`:

```cpp
std::shared_timed_mutex mu;

void reader() {
    std::shared_lock<std::shared_timed_mutex> lk(mu);
    /* mu is shared-locked; lk releases it on scope exit */
}

void writer() {
    std::unique_lock<std::shared_timed_mutex> lk(mu);
    /* mu is exclusively locked */
}
```

The RAII guarantees:

- Constructor acquires (`lock_shared()` for shared_lock,
  `lock()` for unique_lock).
- Destructor releases (`unlock_shared()` / `unlock()`).
- An exception during the critical section still releases
  the lock.

### Why not `std::lock_guard<shared_*mutex>` for readers?

You CAN write
`std::lock_guard<std::shared_timed_mutex> lk(mu)` -- it
compiles and works. But lock_guard always acquires the
**default** lock, which is the EXCLUSIVE lock. Readers using
lock_guard end up serialised, just like writers.

`std::shared_lock` is built specifically for the shared
side: its constructor calls `lock_shared()`, its destructor
calls `unlock_shared()`. That's the only difference, but
it's the important one.

| Wrapper                  | Mode acquired       |
|--------------------------|---------------------|
| `std::lock_guard<M>`     | exclusive (M::lock) |
| `std::unique_lock<M>`    | exclusive (M::lock) |
| `std::shared_lock<M>`    | shared (M::lock_shared) |

If you forget and use lock_guard / unique_lock on the
reader path, the code is **functionally correct** (mutual
exclusion is preserved), but you've thrown away the
parallelism that shared_timed_mutex offered.

### Lifetime patterns

`std::shared_lock` has the same constructors as
`std::unique_lock`:

```cpp
std::shared_lock<MutexType> lk(mu);              /* immediate lock */
std::shared_lock<MutexType> lk(mu, std::defer_lock);    /* don't lock yet */
std::shared_lock<MutexType> lk(mu, std::adopt_lock);    /* assume locked */
std::shared_lock<MutexType> lk(mu, std::try_to_lock);   /* try and fail */
std::shared_lock<MutexType> lk(mu, std::chrono::seconds(1));   /* timed */
```

Most common: just `shared_lock<M> lk(m)` -- acquire on
construction, release on destruction.

### Move-only, not copyable

```cpp
std::shared_lock<MutexType> a(mu);
std::shared_lock<MutexType> b = std::move(a);   /* OK -- transfer ownership */
std::shared_lock<MutexType> c = a;              /* error -- can't copy */
```

The lock represents ownership of a resource (the held-share-
ness of the mutex). Two copies would mean two releases on
destruction -- not what the underlying mutex can handle.

### Manual lock / unlock

You can also defer the lock and acquire it later:

```cpp
std::shared_lock<MutexType> lk(mu, std::defer_lock);
/* ... non-critical setup ... */
lk.lock();
/* ... critical section ... */
lk.unlock();
/* ... other work ... */
lk.lock();    /* re-acquire */
```

`shared_lock` provides `lock()`, `unlock()`,
`try_lock()`, `try_lock_for()`, `try_lock_until()` methods
that forward to the mutex's shared operations.

### `owns_lock()` and bool conversion

```cpp
std::shared_lock<MutexType> lk(mu, std::try_to_lock);
if (lk.owns_lock()) {
    /* got the lock */
} else {
    /* try again later */
}

if (lk) {     /* operator bool() -> owns_lock() */
    /* same thing */
}
```

Useful for the try-or-skip pattern.

### Release without unlocking

`release()` returns the underlying mutex pointer and
disassociates without unlocking:

```cpp
auto *m = lk.release();   /* lk no longer owns the lock; m points to the mutex */
m->unlock_shared();       /* manual unlock */
```

Rarely needed; the common case is just RAII.

### Combined with std::lock for multiple shared mutexes

If you need to lock multiple shared mutexes atomically (to
avoid deadlock), `std::lock` works -- but the deferred-lock
pattern is necessary:

```cpp
std::shared_lock<M> a(mu_a, std::defer_lock);
std::shared_lock<M> b(mu_b, std::defer_lock);
std::lock(a, b);    /* deadlock-free atomic lock of both */
```

This is unusual; most reader-writer code locks one mutex
at a time.

### vs std::unique_lock

| Feature          | unique_lock          | shared_lock          |
|------------------|----------------------|----------------------|
| Mode             | exclusive            | shared               |
| Header           | `<mutex>`            | `<shared_mutex>`     |
| Constructors     | lock/defer/try/adopt/timed | same          |
| Move-only        | yes                  | yes                  |
| `try_lock()`     | yes                  | yes                  |
| `lock()` / `unlock()` | exclusive       | shared               |
| Bool conversion  | yes                  | yes                  |

They're parallel APIs. unique_lock for writers, shared_lock
for readers. Both are interchangeable in
`std::condition_variable_any::wait` arguments.

## Task

In `solution.hpp`, change the reader's lock type from
`std::unique_lock` to `std::shared_lock`:

```cpp
int get(int key) const {
    std::shared_lock<std::shared_timed_mutex> lk(mu_);
    /* ... */
}
```

The writer side stays `std::unique_lock`.

## Run

```sh
./verify.sh
```

## Hints

- One change: in `get()`, swap `std::unique_lock` for
  `std::shared_lock`. Same template argument
  (`std::shared_timed_mutex`).

- Don't change `put()` -- writers correctly use unique_lock.

- `<shared_mutex>` is the header for `std::shared_lock` AND
  `std::shared_timed_mutex`. Already included.

- The stub's bug is the realistic one: when first migrating
  from `std::mutex` to `std::shared_timed_mutex`, people
  often forget that lock_guard / unique_lock acquires the
  EXCLUSIVE side. The reader path needs shared_lock to
  actually benefit from the shared-mutex semantics.

- The peak-concurrent-readers test pins this: with
  shared_lock, multiple readers can hold the lock and the
  peak rises above 1. With unique_lock (stub), readers
  serialise and the peak is 1.

- The RAII-test ("lock_is_raii_released_on_scope_exit") is
  a control check: a sequential put-then-get-then-put-then-
  get on the same map proves the lock is correctly
  released after each operation. With either lock type
  this would work; the test is here to remind that RAII is
  the actual point of shared_lock vs manual
  lock_shared/unlock_shared (step 15's form).

- `std::shared_lock<M>` and `std::unique_lock<M>` are
  parallel APIs. Same constructors (defer_lock /
  adopt_lock / try_to_lock / chrono-timed), same
  bool-conversion, same release semantics.

- Tests verify: all the step-15 properties plus
  "concurrent readers can overlap." Sequential
  put/get/put/get also serves as a deadlock sanity test --
  if the lock weren't released, the second put would
  deadlock.

- C++17 added `std::shared_mutex` (without "timed") and
  recommended `std::shared_lock` work with both. The
  `<shared_mutex>` header gets both types in C++14.
