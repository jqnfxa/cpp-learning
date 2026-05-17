# 114 — `std::unique_lock`

> Spec: <https://en.cppreference.com/cpp/thread/unique_lock>

## What you learn

`std::unique_lock<Mutex>` is the *more flexible* RAII lock
wrapper. Compared to `std::lock_guard` from step 113:

| Feature                          | `lock_guard` | `unique_lock` |
|----------------------------------|--------------|----------------|
| Locks in constructor              | yes          | yes (by default) |
| Unlocks in destructor             | yes          | yes (if owning) |
| **Manual unlock / re-lock**       | no           | **yes**        |
| **Deferred locking** (`defer_lock`) | no           | yes            |
| **Try-locking** (`try_to_lock`)    | no           | yes            |
| **Adopt-locking** (`adopt_lock`)   | yes          | yes            |
| Movable                           | no           | yes            |
| Works with `condition_variable`    | no           | yes (required) |

The headline feature for this step is **manual unlock /
re-lock**:

```cpp
std::mutex m;
std::unique_lock<std::mutex> lk(m);     /* locked */

read_protected_state();                   /* under lock */
lk.unlock();                              /* manually released */

do_expensive_independent_work();          /* other threads can run */

lk.lock();                                /* re-acquire */
write_protected_state();                  /* under lock again */
/* lk's destructor will unlock at scope end */
```

The point: **hold the mutex only when you need it**. If part of
the function can run safely without protection (because it
operates on a snapshot, or doesn't touch shared state), unlock
during that part. Other threads can make progress.

`lock_guard` can't do this -- it locks for the duration of its
scope. `unique_lock` is the tool for "lock, do thing A, unlock,
do thing B, lock, do thing C".

### Other `unique_lock` modes

```cpp
/* 1. Default: lock immediately. */
std::unique_lock<std::mutex> lk1(m);

/* 2. Defer: don't lock yet. Caller may .lock() later. */
std::unique_lock<std::mutex> lk2(m, std::defer_lock);
lk2.lock();

/* 3. Try-lock: non-blocking. lk3.owns_lock() reflects success. */
std::unique_lock<std::mutex> lk3(m, std::try_to_lock);

/* 4. Adopt: caller already locked the mutex; just take responsibility. */
m.lock();
std::unique_lock<std::mutex> lk4(m, std::adopt_lock);
```

For most everyday cases (the default mode + occasional manual
unlock), modes 2-4 are rarely needed. They show up in:
- `std::lock(...)` (lock multiple mutexes deadlock-free,
  deferred).
- `std::condition_variable::wait(unique_lock &)` (requires
  unique_lock, will unlock+wait+re-lock atomically).
- Code that already locked and now wants RAII cleanup
  (adopt_lock).

### The cost of `unique_lock` vs `lock_guard`

`unique_lock` carries one extra bool (`owns_lock` state) on top
of the pointer to the mutex. The destructor checks that bool
before unlocking, so a moved-from or already-unlocked
unique_lock doesn't double-unlock.

The pointer + bool overhead is essentially free. If you don't
need any of unique_lock's extra features, prefer `lock_guard`
for the marginally clearer intent.

### Pattern: snapshot, compute, write back

This is exactly what step 114 asks you to build:

```cpp
template <class WorkFn>
int snapshot_then_compute(int &shared, std::mutex &m, WorkFn work)
{
    std::unique_lock<std::mutex> lk(m);
    int snapshot = shared;
    lk.unlock();                       /* release before expensive work */

    int result = work(snapshot);        /* user-defined "expensive" work */

    lk.lock();                         /* re-acquire for write */
    shared = result;
    return result;
}
```

If `work` itself wants the mutex (e.g., logs through a
shared logger), the released lock during work() lets it
proceed. With `lock_guard` or without manual unlock, the work
function would be unable to acquire the mutex and would either
deadlock or fall back to a "try and skip" path.

The tests exploit this by passing a `work` function that
*does* attempt to acquire the mutex (via `try_lock`). If the
outer code properly released the lock, `try_lock` succeeds. If
not (the stub's case), `try_lock` fails and the result is
sentinel-different.

## Task

In `solution.hpp`, complete the template:

```cpp
namespace task114
{
    /* 1. Lock m via std::unique_lock.
     * 2. Snapshot `shared`.
     * 3. Manually unlock so the user-supplied `work` callable can
     *    use the mutex if it wants to.
     * 4. Call work(snapshot).
     * 5. Re-acquire the lock.
     * 6. Write back work's result to `shared`.
     * 7. Return the new value of `shared` (the destructor unlocks). */
    template <class WorkFn>
    int snapshot_then_compute(int &shared, std::mutex &m, WorkFn work);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body shape:
  ```cpp
  std::unique_lock<std::mutex> lk(m);
  int snapshot = shared;
  lk.unlock();
  int result = work(snapshot);
  lk.lock();
  shared = result;
  return result;
  ```

- `std::unique_lock<std::mutex>` is in `<mutex>`. The template
  parameter `Mutex` matches the type of `m`; using
  `std::unique_lock<std::mutex>` directly is fine. C++17 CTAD
  lets you write `std::unique_lock lk(m);` without the angle
  brackets.

- `lk.unlock()` is the manual unlock. `lk.lock()` is the
  manual lock. Calling `.unlock()` on an already-unlocked
  unique_lock throws `std::system_error`; you can probe
  `lk.owns_lock()` to check first if you're unsure.

- Don't try to use `lock_guard` for this. It can't manually
  unlock. The whole point of the task is using `unique_lock`'s
  extra capability.

- The work callable type is left generic so tests can pass
  different kinds (capture-lambdas, struct-functors). Keep
  `WorkFn` as a template parameter.

- For the test that verifies "the mutex was released during
  work", the test's `work` function does
  `std::unique_lock<std::mutex> inner(m, std::try_to_lock);
  return inner.owns_lock() ? snap * 2 : -1;`. The reference
  returns 10 (= 5 * 2); the stub returns -1 because the outer
  lock was never released.

- Beware of double-unlock: if you write
  `lk.unlock(); ... work(); ... lk.unlock();` the second
  unlock throws. The dtor checks `owns_lock()` before
  unlocking, so the dtor path is safe; manual calls aren't.

- `std::condition_variable::wait(unique_lock &, predicate)` is
  where unique_lock's mutability *really* matters: it unlocks
  the mutex while waiting and re-locks before returning. Step
  118 (condition_variable) builds on this.
