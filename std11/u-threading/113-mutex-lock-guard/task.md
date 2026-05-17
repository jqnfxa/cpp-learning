# 113 — `std::mutex` and `std::lock_guard`

> Spec: <https://en.cppreference.com/cpp/thread/mutex>,
> <https://en.cppreference.com/cpp/thread/lock_guard>

## What you learn

`std::mutex` is the C++11 primitive for **mutual exclusion**:
at most one thread holds it at a time. Its API is two member
functions you almost never call directly:

```cpp
std::mutex m;
m.lock();    /* block until acquired */
/* critical section */
m.unlock();  /* release */
```

You almost never write that pair manually because **the
exception path is awful**:

```cpp
std::mutex m;
m.lock();
do_work();          /* if this throws, mutex stays locked forever */
m.unlock();
```

`std::lock_guard<std::mutex>` is the C++11 RAII wrapper that
locks the mutex in its constructor and **unlocks it in its
destructor**:

```cpp
std::mutex m;
{
    std::lock_guard<std::mutex> guard(m);
    do_work();        /* throws? guard's dtor still unlocks */
}                     /* implicit unlock */
```

This is the standard spelling for "I need the mutex held over
this scope". Three load-bearing rules:

1. **Always RAII**. Bare `m.lock()` / `m.unlock()` calls are a
   smell. Use `lock_guard` (one mutex, simple) or
   `unique_lock` (step 114, more flexible).

2. **Hold the lock for the smallest possible scope**. Build
   data outside the critical section if possible, then enter
   it just to update the shared state.

3. **Never throw while holding the mutex**, even though the
   guard cleans up. Throwing through a critical section leaves
   the protected invariant in an unknown state -- it's a
   correctness hazard, not just a lock hazard.

### The shared-counter race

A `static int counter` (or a member, or any shared variable)
incremented from multiple threads without protection is a data
race. The race shows up as **lost updates**: even on x86, where
`++` looks like one instruction in the source, it compiles to
a `mov`/`add`/`mov` sequence that another thread can interleave
through.

```cpp
int counter = 0;

void worker()
{
    for (int i = 0; i < 100000; ++i)
    {
        ++counter;     /* RACE -- not atomic, lost updates */
    }
}
```

Four threads doing 100,000 increments each: you *expect*
400,000 but you typically *get* something less -- often
significantly less. The exact loss is non-deterministic; it's
where the term "Heisenbug" comes from.

The fix is one wrapped line:

```cpp
int counter = 0;
std::mutex m;

void worker()
{
    for (int i = 0; i < 100000; ++i)
    {
        std::lock_guard<std::mutex> guard(m);
        ++counter;
    }
}
```

Now the increment is serialised. All 400,000 updates land.

### Alternative: `std::atomic<int>`

For the counter case specifically, `std::atomic<int>` (step
116) is faster *and* easier:

```cpp
std::atomic<int> counter{0};
++counter;                /* atomic, no mutex needed */
```

But that only works for primitive types and simple operations.
For "protect a more complex invariant" (a map, a list of
listeners, a chain of state changes), `std::mutex + lock_guard`
is the general tool. Step 113 uses the counter case because
it's the cleanest demo of "race vs no-race"; in real code the
mutex protects something bigger than an int.

### `lock_guard` is not movable

```cpp
std::lock_guard<std::mutex> g1(m);
auto g2 = std::move(g1);   /* compile error */
```

If you need to pass the lock around or unlock partway through,
that's what `std::unique_lock` (step 114) is for. `lock_guard`
is the lightest-weight tool; if you don't need the moves,
prefer it.

## Task

In `solution.hpp`, implement:

```cpp
namespace task113
{
    /* Spawn `n_threads` worker threads. Each one increments a shared
     * counter `iterations` times, under a std::mutex via
     * std::lock_guard. Wait for all threads to finish (join).
     * Return the final counter value (should be n_threads *
     * iterations). */
    int sum_threads(int n_threads, int iterations);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The general shape:
  ```cpp
  int counter = 0;
  std::mutex m;
  std::vector<std::thread> workers;
  workers.reserve(n_threads);

  for (int i = 0; i < n_threads; ++i)
  {
      workers.emplace_back([&] {
          for (int j = 0; j < iterations; ++j)
          {
              std::lock_guard<std::mutex> guard(m);
              ++counter;
          }
      });
  }
  for (auto &t : workers) t.join();

  return counter;
  ```

- `std::lock_guard<std::mutex>` -- the template argument is
  the mutex type. There's only one mutex type in C++11
  (`std::mutex`), so the spelling is fixed. C++17 added class
  template argument deduction (CTAD) which lets you write
  `std::lock_guard guard(m);` without the angle brackets.

- The guard's scope is the body of the inner `for`. As soon
  as the inner `for` iterates, the guard destructs and
  releases the lock for the next thread. That short critical
  section is what makes contention bearable.

- Edge cases:
  - `n_threads == 0`: no workers, no increments, counter is
    0. Trivially correct.
  - `iterations == 0`: workers spawn but don't increment.
    Counter stays 0.
  - Both > 0: counter == n_threads * iterations.

- Tests use 4 threads × 100,000 iterations = 400,000 expected.
  Without locking, the result will be visibly less (typically
  100,000 to 300,000) on a multi-core machine. Single-core
  test environments may *accidentally* produce 400,000
  without locking; the test still passes on the reference and
  the *intent* of the failure mode is documented.

- A clarification on `++` semantics: `counter++` and `++counter`
  are equally racy. The atomicity isn't about source order;
  it's about machine-instruction visibility. Even single-byte
  writes are racy when interleaved with reads.

- `std::lock_guard` constructor takes the mutex by reference.
  It does NOT take a flag to skip locking; if you've already
  locked the mutex elsewhere and need to *adopt* the lock,
  you'd use `std::lock_guard(m, std::adopt_lock)` -- not
  relevant for this task.
