# 65 — `thread_local` storage

> Spec: <https://en.cppreference.com/cpp/language/storage_duration#Thread_storage_duration>

## What you learn

C++11 added a fourth storage duration alongside `static`,
`automatic`, and `dynamic`: **thread storage duration**. A
`thread_local` variable has *one instance per thread* — created
when the thread first uses it (or at thread startup, depending on
the variable kind) and destroyed when the thread ends.

```cpp
thread_local int counter = 0;   /* one `counter` per thread */
```

From inside one thread the variable behaves exactly like a normal
`static` (same lifetime within the thread, same name, same
linkage rules). What you *don't* see is the other threads'
copies. That makes `thread_local` the right tool for:

- Per-thread caches / scratch buffers.
- Per-thread RNG state.
- Per-thread error codes (think `errno`, which on modern POSIX
  *is* a thread-local).
- Anything that would otherwise force a mutex purely because of
  shared mutable state.

### Lifetime, in one paragraph

A `thread_local` at namespace scope is initialised at or before
its first odr-use within each thread, and destroyed when the
thread exits — in reverse order of their construction. This
matches static initialisation order rules, scoped to a thread.
Local `thread_local` statics inside a function are initialised
the first time control passes their declaration *in that thread*.

### Combining with other specifiers

`thread_local` can combine with `static` (at namespace scope this
is implied; in a class context `static thread_local` is how you
spell a per-thread static data member) and with `extern` to
declare without defining.

```cpp
struct counter
{
    static thread_local int value;   /* declaration */
};
thread_local int counter::value = 0; /* definition (one per TU) */
```

### Gotchas

- Address comparison: `&x` evaluated in two different threads
  yields different pointers. Don't cache the address across
  threads.
- Initialisation cost: a `thread_local` with a non-trivial
  constructor pays a hidden check on each access in many
  implementations (the "is it initialised yet?" guard). For hot
  paths, prefer trivial types or POD-ish state.
- Not the same as `std::atomic`: `thread_local` removes the
  sharing, atomics make sharing safe. They solve different
  problems.

## Task

In `solution.hpp`, declare and define a function that returns a
reference to a `thread_local` counter, plus a small API around it:

```cpp
namespace task65
{
    int &counter() noexcept;       /* reference to the per-thread counter */
    int  increment() noexcept;     /* ++counter() and return the new value */
    int  read() noexcept;          /* return counter() */
    void reset() noexcept;         /* counter() = 0 */
}
```

The counter must be a function-local `thread_local int`
initialised to `0`. Returning a *reference* lets tests confirm
that the address differs between threads.

Tests will:

- Increment the counter in the main thread several times and
  observe the value grow.
- Spawn a fresh thread, observe that thread's counter starts at
  0 (independent from main), increment it, then check the main
  thread's counter is unaffected.
- Spawn many threads in parallel, each incrementing its own
  counter to a known value; assert each one sees exactly its own
  count.
- Compare `&counter()` across threads; assert the addresses
  differ.

## Run

```sh
./verify.sh
```

## Hints

- A function-local `thread_local int n = 0;` returned by reference
  is one short function. Same shape as a Meyers' singleton, but
  per-thread instead of per-program.

- `std::thread t([] { ... }); t.join();` is enough for the
  threading bits — no condition variables needed.

- To prove addresses differ, capture `&task65::counter()` inside
  the thread lambda and compare to a copy taken in the main
  thread. They must not be equal.

- This task has no synchronisation in it. There is nothing to
  synchronise — each thread has its own object, and there is no
  cross-thread access to that object. Adding a mutex would be a
  red flag in review.

- Resetting between tests matters: gtest tests can share a
  process. The main thread's counter persists across tests, so
  call `reset()` (or set counter() = 0) where appropriate.
