# 121 — `std::packaged_task`

> Spec: <https://en.cppreference.com/cpp/thread/packaged_task>

## What you learn

`std::packaged_task<Signature>` is a **callable wrapped in a
promise**. You give it a callable; you get back something that
(a) you can call like a function, and (b) has a `.get_future()`
that delivers the call's result.

```cpp
std::packaged_task<int(int)> task([](int x) { return x * 2; });
std::future<int> f = task.get_future();

task(21);              /* run the task; sets the promise */
int v = f.get();       /* 42 */
```

Three things to internalise:

1. **It's a callable.** `task(args...)` runs the wrapped
   callable, then **stores the result** (or exception) in
   the associated future.
2. **The future is one-shot.** `task.get_future()` may be
   called once; `f.get()` may be called once.
3. **Move-only.** You can't copy a packaged_task. You move it
   (typically into a thread or a queue).

### vs `std::async` (step 120)

`std::async` does the "build a packaged_task, spawn a thread,
return the future" combo in one step. `std::packaged_task` is
the **decomposed** version -- you build the task, hand it
somewhere, and the thing on the other side calls it when
it's ready.

The common case where this matters: a **thread pool**. You
have N worker threads pulling tasks from a queue. The "task"
is a `std::packaged_task<...>` that the pool runs when it has
capacity. The caller gets back a `std::future` to wait on.
`std::async`'s "spawn a fresh thread each call" doesn't fit
this model.

```cpp
/* Pseudocode: simple "enqueue this and return its future" */
template <class F>
std::future<int> enqueue(F fn, int x)
{
    std::packaged_task<int(int)> task(fn);
    std::future<int> f = task.get_future();
    std::thread(std::move(task), x).detach();   /* runs somewhere */
    return f;
}
```

The detach is "fire and forget the running" -- the future
still carries the result. The wrapped task self-cleans:
when it finishes, the result is in the future; the underlying
thread exits.

### Exception transfer (still automatic)

If the callable throws, `packaged_task` catches the exception
and sets it on the associated future via `std::exception_ptr`.
`f.get()` rethrows on the caller's side. Same protocol as
step 96 (raw exception_ptr), 119 (promise/future), and 120
(async).

### `reset()`

After `task(args)` has run once, the task is "done" -- calling
it again is UB. To re-use the *same* task with a new future:

```cpp
task.reset();   /* re-arms; .get_future() returns a fresh future */
```

The wrapped callable persists; the result-storage is fresh.
Not part of this task, but useful in real pools.

## Task

In `solution.hpp`, implement a function template:

```cpp
namespace task121
{
    /* Wrap fn in a std::packaged_task<int(int)>, spawn a worker
     * thread that runs the task with x, and return the future.
     * The worker is detached. */
    template <class F>
    std::future<int> enqueue_task(F fn, int x);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body shape:
  ```cpp
  std::packaged_task<int(int)> task(fn);
  std::future<int> f = task.get_future();
  std::thread(std::move(task), x).detach();
  return f;
  ```

- `std::packaged_task<int(int)>` is in `<future>`. Include it.

- The task's *Signature* is the function-type the task wraps:
  `int(int)` means "takes one int, returns int". The wrapped
  `fn` must match this signature (the std::function-like
  type-erasure makes "any compatible callable" work).

- `std::move(task)` because `packaged_task` is move-only. The
  thread constructor takes the moved task; subsequent uses of
  the local `task` are invalid (it's been moved-from).

- The detached worker keeps the task alive for the duration of
  the call. When the task finishes, its destructor runs in
  the worker thread; the future already has the result; the
  worker thread exits.

- Detaching is fire-and-forget: the test process's `main`
  must finish for the detached threads to be reaped (the OS
  cleans them up at exit). For unit tests this is fine; for
  production, prefer a thread pool that explicitly tracks
  workers.

- Tests verify return value, exception transfer (a throwing
  fn still surfaces on the main thread via `.get()`), and
  multi-call independence.

- The packaged_task -> thread -> future pipeline is the
  building block for thread pools. Knowing this pattern is
  knowing how `boost::asio::thread_pool`, libdispatch's
  blocks, and (loosely) Python's `concurrent.futures.Executor`
  work under the hood.
