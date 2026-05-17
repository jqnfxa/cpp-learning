# 117 — `std::atomic<T>`

> Spec: <https://en.cppreference.com/cpp/atomic/atomic>

## What you learn

`std::atomic<T>` is C++11's primitive for **atomic
operations on shared data**. Where step 113 used
`std::mutex + lock_guard` to serialise increments, this step
uses `std::atomic<int>::fetch_add` -- a single hardware
instruction (on most CPUs, `lock add` on x86) that does
read-modify-write **without a lock**.

```cpp
std::atomic<int> counter{0};

void worker()
{
    for (int i = 0; i < 100000; ++i)
    {
        counter.fetch_add(1);   /* atomic ++ */
    }
}
```

Four threads doing 100,000 fetch_adds each: result is exactly
400,000. No data race, no lost updates, no mutex.

### How is this different from `++counter` on a plain `int`?

- **Plain int**: the compiler emits a load, an add, and a
  store. Three separate operations, interleavable. Lost
  updates.
- **`std::atomic<int>::fetch_add`**: the compiler emits a
  single CPU instruction that combines all three with
  hardware-level locking on the cache line. No interleaving.
  Guaranteed correct.

The atomic version is faster than mutex + lock_guard for
single-value operations: no syscalls, no kernel-level
contention, just a CPU memory barrier.

### When to use atomic vs mutex

| Scenario                          | Use            |
|-----------------------------------|----------------|
| Counter, flag, single pointer     | `std::atomic`  |
| Map, list, complex invariant      | `std::mutex`   |
| Set of related fields              | `std::mutex`   |
| Wait for a condition               | `cv + mutex`   |

`std::atomic` shines for *primitive types* and *single
operations*. The moment you need "lock these N things together"
or "check condition X then do Y" as one transaction, you
want a mutex.

### Operations on `std::atomic<int>`

| Operation                  | Effect                            |
|----------------------------|-----------------------------------|
| `store(v)`                  | a = v                              |
| `load()`                    | returns a                          |
| `exchange(v)`               | swap; returns old value            |
| `fetch_add(n)`              | a += n; returns old value          |
| `fetch_sub(n)`              | a -= n; returns old value          |
| `compare_exchange_weak/strong` | atomic CAS (step 118 territory) |
| `operator++` / `operator+=` | sugar for `fetch_add(1)` /  `fetch_add(n)` |

All of these are atomic in the C++11 memory model sense. They
also accept an optional `std::memory_order` parameter (step
118 covers that); without it, the default is `seq_cst`
(strongest, slowest).

### Defaults and quirks

- `std::atomic<T>` for **integral** and **pointer** types is
  always lock-free on commodity hardware. For larger types
  (`std::atomic<MyStruct>`), the compiler may emit a hidden
  mutex if the type doesn't fit in a single cache line.
  Probe with `std::atomic<MyStruct>::is_always_lock_free`
  (C++17) or `is_lock_free()` (C++11, runtime).
- **No copy/assign** between atomics:
  `std::atomic<int> a = b;` is ill-formed. Use `a.store(b.load())`.
- **Initialisation** is *not* atomic. Default-construct the
  atomic; don't rely on its initial value being visible to
  threads without external synchronisation.

### The contrast with step 113

Step 113 protected the counter with `std::mutex + lock_guard`.
The result was correct but slow (mutex overhead on every
increment). This step does the same job with `std::atomic` --
correct *and* fast.

For "single integer counter incremented from many threads",
`std::atomic<int>` is almost always the right answer.

## Task

In `solution.hpp`, implement the same `sum_threads` API as
step 113, but using `std::atomic<int>::fetch_add` instead of
a mutex:

```cpp
namespace task117
{
    /* Spawn n_threads worker threads; each calls fetch_add(1) on a
     * shared std::atomic<int> `iterations` times. Wait for all
     * threads, return the final counter value. */
    int sum_threads(int n_threads, int iterations);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The shape:
  ```cpp
  std::atomic<int> counter{0};
  std::vector<std::thread> workers;
  workers.reserve(n_threads);

  for (int i = 0; i < n_threads; ++i)
  {
      workers.emplace_back([&] {
          for (int j = 0; j < iterations; ++j)
          {
              counter.fetch_add(1);
          }
      });
  }
  for (auto &t : workers) t.join();

  return counter.load();
  ```

- `std::atomic<int>` is in `<atomic>`. Include it.

- The constructor `std::atomic<int> counter{0}` *initialises*
  the atomic with 0. The C++11 atomic constructor is NOT
  atomic itself, but no other thread has observed it yet so
  the initial store is fine.

- `counter.load()` reads the final value (the test returns
  this). You could also use `counter.exchange(0)` (atomic read-
  and-clear) but plain `load` is what you want here.

- `counter.fetch_add(1)` is equivalent to `++counter` on a
  `std::atomic<int>`. The explicit form makes "this is atomic"
  obvious to a reader.

- Performance side-note: for n_threads = 4 and iterations =
  100000, the atomic version finishes in microseconds. The
  mutex version (step 113) takes milliseconds. The same
  workload, different primitives.

- For sequences of operations that need to atomic *together*
  (e.g. "if counter > N then reset"), `compare_exchange` is
  the building block. The `compare_exchange_weak/strong` pair
  is the heart of lock-free data structures; not part of
  this task.
