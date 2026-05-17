# 118 — memory orderings

> Spec: <https://en.cppreference.com/cpp/atomic/memory_order>

## What you learn

C++11's atomics take an optional `std::memory_order` parameter
that controls **how the operation synchronises with other
threads**. The default for every atomic operation is
`std::memory_order_seq_cst` (sequential consistency) — the
strongest, the safest, the slowest.

For performance, you can sometimes use weaker orderings. The
key pair for inter-thread handoffs is **release/acquire**:

```cpp
std::atomic<int>  data{0};
std::atomic<bool> flag{false};

/* producer thread */
data.store(42, std::memory_order_relaxed);
flag.store(true, std::memory_order_release);     /* (1) */

/* consumer thread */
while (!flag.load(std::memory_order_acquire)) { } /* (2) */
int v = data.load(std::memory_order_relaxed);     /* sees 42 */
```

The contract: **a release store synchronises with an acquire
load that reads the value written**. That synchronisation
makes every memory write *before* the release visible to every
memory read *after* the acquire. The compiler and CPU are
constrained from reordering across the (1)/(2) pair.

### Why `relaxed` would be wrong

Replace the (1)/(2) memory orders with `relaxed`:

```cpp
flag.store(true, std::memory_order_relaxed);     /* WRONG */
while (!flag.load(std::memory_order_relaxed)) { } /* WRONG */
```

`memory_order_relaxed` provides **atomicity but no
synchronisation**. The single store/load is indivisible (no
torn writes), but operations *around* it can be reordered
freely by the compiler and CPU.

The bug: with relaxed, the consumer might read `flag == true`
and then read `data == 0` (the old value). The producer's
`data` write isn't ordered before the `flag` write under
relaxed; the consumer's `data` read isn't ordered after the
`flag` read. The hardware (especially on weakly-ordered CPUs
like ARM) is free to reorder.

On x86, with its strongly-ordered memory model, this
particular reordering rarely happens at the hardware level.
But the C++ standard's allowance is what matters: any compiler
optimisation that respects the *abstract machine* under
relaxed is legal. A compiler could hoist or sink the data
access. The bug may stay dormant on x86 and explode on ARM.

### The four common orderings

| Ordering          | What it does                                              |
|-------------------|-----------------------------------------------------------|
| `seq_cst`         | Default. Strongest. Total global ordering of all seq_cst ops |
| `acquire`/`release` | Pair-wise: release writes published, matched acquire reads see them |
| `acq_rel`         | Both acquire and release semantics (for RMW ops like fetch_add) |
| `relaxed`         | Atomic but no synchronisation across the op                |

For most code, `seq_cst` is the right default -- it's correct
and the cost is small compared to the actual contention.
`release`/`acquire` shows up when profiling reveals seq_cst
fences as a bottleneck on weakly-ordered hardware.

`relaxed` is the danger zone: legitimate for counters (where
you don't care about ordering, only the sum), wrong for
flag-based handoffs (where ordering is the whole point).

### The classic handoff pattern

This step asks you to implement the textbook handoff:

```cpp
int handoff(int payload)
{
    std::atomic<int>  data{0};
    std::atomic<bool> flag{false};
    int observed = -1;

    std::thread consumer([&] {
        while (!flag.load(std::memory_order_acquire))
            std::this_thread::yield();
        observed = data.load(std::memory_order_relaxed);
    });

    data.store(payload, std::memory_order_relaxed);
    flag.store(true,    std::memory_order_release);

    consumer.join();
    return observed;
}
```

Key parts:
- **Producer writes data first, then flag**. The "happens-
  before" chain depends on this order.
- **Producer's flag store is release**. Everything before this
  store (in program order) is published.
- **Consumer's flag load is acquire**. Once it returns true,
  everything that was before the matching release in the other
  thread is visible.
- **Producer's data store and consumer's data load can be
  relaxed** -- the synchronisation comes from the release/
  acquire pair on `flag`.

## Task

In `solution.hpp`, implement:

```cpp
namespace task118
{
    /* Spawn a consumer thread that waits on a flag, then reads a
     * payload. Producer (the calling thread) writes the payload,
     * then sets the flag with release. Consumer waits with acquire.
     * Returns the value the consumer observed. */
    int handoff(int payload);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body shape:
  ```cpp
  std::atomic<int>  data{0};
  std::atomic<bool> flag{false};
  int observed = -1;

  std::thread consumer([&] {
      while (!flag.load(std::memory_order_acquire))
          std::this_thread::yield();
      observed = data.load(std::memory_order_relaxed);
  });

  data.store(payload, std::memory_order_relaxed);
  flag.store(true,    std::memory_order_release);

  consumer.join();
  return observed;
  ```

- The spin loop with `std::this_thread::yield()` is the cheap
  cooperative-wait. For "real" wait, use a `condition_variable`
  (step 116) -- but for the memory-order demo, the spin loop
  is what makes the load/store ordering visible.

- `std::this_thread::yield` is in `<thread>`. It hints to the
  scheduler to let other threads run; in practice on Linux,
  it's a syscall to the scheduler.

- The runtime tests can't easily *prove* the memory-order
  correctness on x86 (because x86 doesn't reorder this
  particular pattern at the hardware level). What the tests
  do verify is that the handoff function returns the right
  value, every time, even under heavy stress.

- The task.md goes deep on *why* relaxed is wrong; the tests
  go shallow on *that* it works. For demonstrating actual
  memory-order bugs you need either weakly-ordered hardware
  or a memory-model fuzzer (e.g., relacy, herd7). Those are
  out of scope here.

- C++17 added `std::atomic_thread_fence` and `std::atomic_signal_fence`
  for standalone fences. C++20 added `std::atomic::wait` /
  `notify_one` which gives condition_variable-like semantics
  without a mutex. For C++11, this is the toolkit.
