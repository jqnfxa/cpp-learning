# 66 — the C++11 memory model (conceptual)

> Spec: <https://en.cppreference.com/cpp/atomic/memory_order>,
> <https://en.cppreference.com/cpp/language/memory_model>

## What you learn

C++11 was the first standard to define what *concurrent* programs
mean. Before C++11, threading was a platform extension; the
language pretended programs were single-threaded. The 2011
standard introduced a **memory model** — a contract about which
reads can see which writes when multiple threads run, and what
counts as undefined behaviour.

The single most important definition is the **data race**:

> Two evaluations on different threads are a *data race* if
> (a) at least one is a write, (b) they access the same memory
> location, and (c) they are not ordered by any
> *synchronises-with* relation (no `std::mutex`, no atomic
> happens-before, no thread `join`, etc.).

A data race is undefined behaviour. The compiler is allowed to
assume your program has none; the optimiser will gladly produce
nonsense code if you do.

Things that count as synchronisation (and thus *break* a race):

- Locking and unlocking the same `std::mutex` around the accesses.
- Reading/writing the same `std::atomic` (any memory order).
- A thread's *end of execution* synchronises with `t.join()` in
  the joiner.
- Initialisation of a function-local `static` synchronises with
  the threads that observe it initialised (this is what guards
  Meyers singletons).

Things that **do not** synchronise:

- `volatile`. (It does *not* mean atomic. It is a historical
  spelling used for memory-mapped I/O.)
- `std::cout`, `printf`, system calls. These are not "barriers"
  in the memory-model sense.
- "Two threads both only read a non-atomic" — that's allowed,
  because no operation is a write. *Not* a race.

### Quick taxonomy you should be able to do at a glance

| Scenario                                                | Race?  | Why                                                    |
|---------------------------------------------------------|--------|--------------------------------------------------------|
| Both threads only read a plain `int`                    | No     | No write → no race by definition                       |
| One thread writes, another reads a plain `int`, no sync | **Yes**| Write + concurrent read, no synchronisation            |
| Two threads write to different `int`s                   | No     | Different memory locations                             |
| One writes, one reads `std::atomic<int>`                | No     | Atomics provide the synchronisation themselves         |
| Reads/writes inside the same `std::mutex` lock          | No     | Mutex creates happens-before                           |
| Two threads concurrently write a plain `int`            | **Yes**| Two writes to the same location, no sync               |

The "no sync" line is what to look for in code review: a shared
mutable variable touched by two threads with nothing between
them is the textbook race.

### Why this matters more than the API

You can memorise the atomic API in a day. The memory model is
what tells you whether a *program* is correct — independent of
the API you used. Most concurrency bugs in real C++ codebases
are races that compile fine and pass most runs.

## Task

In `solution.hpp`, fill in six `constexpr bool` answers, one per
scenario, marking whether the snippet contains a data race as
defined by the C++11 memory model.

The skeleton:

```cpp
namespace task66
{
    /* Two threads both only do `(void)shared;` -- two non-atomic reads. */
    constexpr bool scenario_1_both_read_plain   = /* TODO */;

    /* Thread A writes `shared = 1;`, thread B reads `int x = shared;`.
     * Plain `int`, no synchronisation. */
    constexpr bool scenario_2_write_then_read   = /* TODO */;

    /* Thread A writes `a = 1;`, thread B writes `b = 2;` -- two
     * different plain ints, no synchronisation. */
    constexpr bool scenario_3_disjoint_writes   = /* TODO */;

    /* Thread A does `shared.store(1)`, thread B does `shared.load()` --
     * `shared` is `std::atomic<int>`. */
    constexpr bool scenario_4_atomic_pair       = /* TODO */;

    /* Both threads lock `m`, then read/write `shared`, then unlock. */
    constexpr bool scenario_5_mutex_protected   = /* TODO */;

    /* Two threads both do `++shared;` on a plain `int`, no sync. */
    constexpr bool scenario_6_concurrent_writes = /* TODO */;
}
```

Set each `bool` to `true` if the scenario *is* a data race under
the C++11 memory model, `false` otherwise.

The tests are six `static_assert`s — one per scenario — that
will refuse to compile if your classification is wrong.

## Run

```sh
./verify.sh
```

## Hints

- This task has no runtime work and no threads. The point is the
  classification — the answers are pinned at compile time.

- Recipe:
  1. Is there at least one *write*? If no → no race (read-read is
     fine).
  2. Are the two accesses on the *same* memory location? If no →
     no race.
  3. Is there a synchronisation between them (mutex, atomic on
     the same object, join, etc.)? If yes → no race.
  4. Otherwise → race.

- Common confusion: `std::atomic` doesn't just make a *write*
  atomic, it also provides the *synchronisation* needed to pair
  with reads of the same atomic. Both halves matter — that's
  why scenario 4 is not a race.

- Read 2 and 6 carefully. Scenario 2 is "one writer, one reader,
  no sync". Scenario 6 is "two writers, no sync". Both are
  races; the spec definition fires the moment *any* of the
  concurrent operations is a write.

- `volatile int` does not change any of these answers. If you
  caught the temptation, good — that confusion is exactly what
  the C++11 model was trying to clear up.

- The "release / acquire / seq_cst" choices are about *what*
  synchronisation an atomic gives you. For this task the
  question is only "is there any synchronisation at all", which
  is at the *previous* level of detail. Memory orders come up
  later in section L.
