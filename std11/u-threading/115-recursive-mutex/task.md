# 115 — `std::recursive_mutex`

> Spec: <https://en.cppreference.com/cpp/thread/recursive_mutex>

## What you learn

A plain `std::mutex` can be locked at most *once* per thread. A
thread that tries to acquire a mutex it already holds gets
**undefined behaviour** -- in practice, a deadlock against
itself:

```cpp
std::mutex m;

void f() {
    std::lock_guard<std::mutex> g(m);  /* take the lock */
    g_internal();                       /* if this also locks m -> deadlock */
}

void g_internal() {
    std::lock_guard<std::mutex> g(m);  /* same thread, same mutex -> hangs */
}
```

`std::recursive_mutex` lifts that restriction. The same thread
may lock it any number of times, **as long as the number of
unlocks matches**. Other threads see it as held until the last
unlock.

```cpp
std::recursive_mutex m;

void f() {
    std::lock_guard<std::recursive_mutex> g(m);  /* lock count = 1 */
    g_internal();                                 /* lock count = 2; OK */
}                                                  /* unlocks count = 0 */

void g_internal() {
    std::lock_guard<std::recursive_mutex> g(m);  /* lock count = 2 */
}                                                  /* unlocks count = 1 */
```

### Why this exists

The common motivation: a class with several public methods that
all need the lock, and some public method calls another.

```cpp
class counter
{
    int value_ = 0;
    std::recursive_mutex m_;

public:
    void increment()
    {
        std::lock_guard<std::recursive_mutex> g(m_);
        ++value_;
    }

    void increment_twice()
    {
        std::lock_guard<std::recursive_mutex> g(m_);
        increment();        /* recursive lock -- needs recursive_mutex */
        ++value_;
    }
};
```

Without `recursive_mutex`, you'd refactor: split each public
method into a public "locks then calls private" and a private
"already locked, does the work" version. That's the **non-
recursive private helper** pattern, and many codebases prefer
it over `recursive_mutex` because it makes the
"who-locks-what" contract explicit.

### Why `recursive_mutex` is often a code smell

Reasons to prefer the private-helper refactor:

- **Performance**: `recursive_mutex` is slower than `mutex`
  (tracks owner + count).
- **Clarity**: with `mutex`, the lock-once rule is enforced;
  the lock graph is auditable.
- **Discipline**: a public method that calls another public
  method while holding a lock is a fragile design. Splitting
  out a private "already_locked" helper makes the contract
  explicit.

But sometimes you have a polymorphic dispatch where the
re-entry happens through a virtual call, or a library callback
that re-enters your code, and refactoring isn't an option. In
those cases, `recursive_mutex` is the right tool.

### Lifecycle parity

The lock count must reach zero in the same thread that incremented
it. Two unlocks for two locks. RAII (lock_guard / unique_lock)
makes this automatic: each guard increments on construction,
decrements on destruction.

```cpp
{
    std::lock_guard<std::recursive_mutex> a(m);   /* count: 1 */
    {
        std::lock_guard<std::recursive_mutex> b(m);   /* count: 2 */
    }                                                  /* count: 1 */
}                                                      /* count: 0 -- now released to others */
```

## Task

In `solution.hpp`, complete the `counter` class so that:

- It has an internal mutex of the **right kind** to allow
  `increment_twice()` to recursively call `increment()`.
- `increment()` locks the mutex and increments the value by 1.
- `increment_twice()` locks the mutex, calls `increment()`,
  then increments by 1 again. Total: +2.
- `read()` returns the value under the lock.

The scaffold ships with **`std::mutex`** as the mutex type --
the TODO is to swap that for `std::recursive_mutex`.

## Run

```sh
./verify.sh
```

## Hints

- The fix is a one-line change to the member declaration:
  ```cpp
  std::recursive_mutex m_;
  ```
  And then the `lock_guard`s also need their type parameter
  changed:
  ```cpp
  std::lock_guard<std::recursive_mutex> g(m_);
  ```

- `std::recursive_mutex` is in `<mutex>`, same header as
  `std::mutex`.

- The tests use a watchdog pattern: each call that triggers
  recursion runs in a worker thread, and the test thread waits
  up to 500ms for completion. If the call deadlocks (stub
  case), the watchdog declares failure and detaches the worker
  to let the test process keep running.

- The detached worker leaks (it's blocked forever on the
  inner lock). For the duration of a single ctest run, the
  leaked threads are harmless -- the test process exits after
  reporting failure and the OS cleans them up.

- Don't try to write your own "release-and-relock" dance in
  `increment_twice` to avoid the recursive lock. The point of
  this task is `recursive_mutex`; sidestepping it defeats the
  exercise.

- In production: if you find yourself reaching for
  `recursive_mutex`, consider the "private helper" refactor
  first. The recursive lock is the right tool when the
  re-entry is through a callback or polymorphic dispatch that
  you can't restructure.

- C++17 added `std::scoped_lock` which generalises lock_guard
  to multiple mutexes deadlock-free. It's compatible with both
  `mutex` and `recursive_mutex`. For C++11 you stick with
  `lock_guard<recursive_mutex>`.
