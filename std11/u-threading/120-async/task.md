# 120 — `std::async` and launch policies

> Spec: <https://en.cppreference.com/cpp/thread/async>

## What you learn

`std::async` is the high-level wrapper that packages step 119's
promise/future dance into a one-liner. You pass it a callable;
it returns a `std::future<T>` that will hold the result.

```cpp
std::future<int> f = std::async(do_work, x);
int v = f.get();   /* blocks until done */
```

The two-argument form takes an explicit **launch policy**:

```cpp
std::async(std::launch::async,    fn, x);    /* run on a new thread now */
std::async(std::launch::deferred, fn, x);    /* don't run until .get() / .wait() */
std::async(fn, x);                            /* default: implementation chooses */
```

### `std::launch::async` — run immediately on a new thread

The callable starts running **immediately** on a fresh thread.
The returned future will be fulfilled when the thread finishes.
This is what you get if you want true parallelism.

```cpp
std::atomic<bool> started{false};
auto f = std::async(std::launch::async, [&] {
    started = true;
    /* ... */
});
/* `started` becomes true almost immediately, even before f.get() */
```

### `std::launch::deferred` — run lazily on the caller's thread

The callable is **stored** but not started. It runs only when
something queries the future — `f.get()`, `f.wait()`,
`f.wait_for()`, `f.wait_until()`. And it runs **on the
querying thread**, not a new one.

```cpp
std::atomic<bool> started{false};
auto f = std::async(std::launch::deferred, [&] {
    started = true;
    return 42;
});
/* `started` is still false here. */
sleep_a_while();
/* still false. */
int v = f.get();   /* NOW the callable runs, on this thread. v == 42. */
/* `started` is now true. */
```

The trade-off: zero threading overhead, no parallelism. It's
like `std::function` with futures glue. Useful when you might
not actually need the value, or when you want lazy computation
but with the future-API uniformity.

### The default policy — `async | deferred`

When you call `std::async(fn, x)` with no policy, the
implementation is allowed to pick **either**. In practice on
common platforms the implementation picks `async`, but the
standard doesn't require it. Code that depends on "is this
running concurrently?" must specify `std::launch::async`
explicitly.

```cpp
auto f = std::async(slow_io);    /* may or may not parallelise */
/* ... do other work ... */
auto v = f.get();                /* if it was deferred, NO parallelism happened */
```

This is one of those C++11 corners where the convenience form
is subtly wrong for performance-sensitive code. Always pick the
policy explicitly.

### Exception transfer (same as step 119)

If the callable throws, `f.get()` rethrows the same exception
on the caller's side, preserving the dynamic type. The
mechanism is exactly the promise/future plumbing from step 119
— `std::async` just sets it up for you.

### The future destructor blocks (for async)

A subtle but important rule: when a `std::future` returned by
`std::async(std::launch::async, ...)` is **destroyed without
get()/wait()**, the destructor **blocks** until the
asynchronous task completes:

```cpp
{
    auto f = std::async(std::launch::async, slow_work);
}   /* `f`'s destructor blocks here, waiting for slow_work to finish */
```

This is unique to `std::async`-returned futures (futures from
`promise::get_future()` don't have this behaviour). It's
intended to make "fire and forget" hard to do by accident.

The right way to actually not wait: assign the future to
something that lives longer, or move it into a container. The
"async destructor blocks" gotcha trips people up; just be
aware and explicit.

### `std::launch::deferred` futures don't block on destruct

The deferred path stores the callable but doesn't run it. If
you destroy the future without calling get(), the deferred
work is **discarded** — never runs. No block, no execution.

## Task

In `solution.hpp`, implement two function templates:

```cpp
namespace task120
{
    /* Launch fn(x) immediately on a new thread.  Returns the future. */
    template <class F>
    std::future<int> spawn_async(F fn, int x);

    /* Defer fn(x) until the future's get/wait is called.  Returns the
     * future. */
    template <class F>
    std::future<int> spawn_deferred(F fn, int x);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `spawn_async`:
  ```cpp
  return std::async(std::launch::async, fn, x);
  ```

- `spawn_deferred`:
  ```cpp
  return std::async(std::launch::deferred, fn, x);
  ```

- Both functions are one-line wrappers around `std::async`.
  The point is choosing the right launch policy.

- `std::async` is in `<future>`. Include it.

- The tests probe behaviour by setting a `std::atomic<bool>`
  flag inside the callable. With `launch::async`, the flag
  becomes true almost immediately (parallel start). With
  `launch::deferred`, the flag stays false until `f.get()` is
  called.

- The stub uses `std::launch::async` for both, so the
  "deferred-doesn't-start-until-get" test fails (the deferred
  callable actually starts immediately).

- Don't use the no-policy form `std::async(fn, x)` for either
  function -- it's the "implementation chooses" form, which
  the tests can't deterministically distinguish.

- Note the `std::future` destructor-blocks rule for async-
  returned futures. The tests return the future from the
  function and let the test scope own it; the destructor
  blocks at scope exit if `f.get()` wasn't called.

- C++20 added `std::jthread` and coroutine support, but
  `std::async` is still the simplest way to fire a one-shot
  async task. It survives as the "throw a callable at the
  runtime" entry point.
