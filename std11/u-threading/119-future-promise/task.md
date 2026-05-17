# 119 — `std::future` and `std::promise`

> Spec: <https://en.cppreference.com/cpp/thread/future>,
> <https://en.cppreference.com/cpp/thread/promise>

## What you learn

`std::promise<T>` and `std::future<T>` are C++11's **value-
synchronisation primitives**: a one-shot channel between two
threads where one *fulfils* a value and the other *waits* for
it.

```cpp
std::promise<int>  p;
std::future<int>   f = p.get_future();    /* same shared state */

/* in producer thread */
p.set_value(42);                           /* fulfil */

/* in consumer thread */
int v = f.get();                           /* blocks until set; returns 42 */
```

Three things to internalise:

1. **They share state** through `p.get_future()`. Each side
   manipulates the *same* hidden control block.
2. **One-shot**: `set_value` can only be called once on a given
   promise. `get` can only be called once on a given future
   (it moves the value out).
3. **Both halves are move-only**. You move a `std::future` into
   a callable to give it consumer rights; you move a promise
   to give a producer rights.

The classic use is "spawn a worker thread to compute something,
get the result back on the main thread":

```cpp
int compute_async(int x)
{
    std::promise<int> p;
    std::future<int>  f = p.get_future();

    std::thread worker([&p, x] {
        try { p.set_value(do_work(x)); }
        catch (...) { p.set_exception(std::current_exception()); }
    });

    worker.join();
    return f.get();                /* now ready */
}
```

### Why not just shared variables + a condition_variable?

You could, and people did before C++11. The future/promise
pair is the *value-typed* version of that protocol:

- It packages the "ready" flag and the value together.
- It handles exceptions: a worker that throws can call
  `set_exception` and the main thread's `f.get()` rethrows.
- It's move-only, so the type system enforces single-consumer
  semantics.

Compared to step 96 (exception_ptr + thread): future/promise
is the same protocol with value-passing built in. Compared to
condition_variable: less boilerplate, type-safe.

### Exception transfer

If the worker throws, the main thread's `.get()` rethrows on
*its* side -- preserving the dynamic type. The pattern is:

```cpp
std::thread worker([&p] {
    try {
        p.set_value(do_work());
    } catch (...) {
        p.set_exception(std::current_exception());
    }
});
```

The `catch (...) p.set_exception(current_exception())` block
captures whatever was thrown and passes the
`std::exception_ptr` to the future. `f.get()` then rethrows it
on the main thread.

### `std::async` is the shortcut

For "run this callable on a new thread and give me the result
as a future", C++11 already provides `std::async`:

```cpp
std::future<int> f = std::async(std::launch::async, do_work);
int v = f.get();
```

`std::async` does the promise/future plumbing for you. You
write the explicit pair when you need more control --
custom-managed threads, multi-step pipelines, or a future
you'll hand off to N other threads.

### Capture by reference vs by move

In C++11 the lambda init-capture (`[p = std::move(p)]`) doesn't
exist (it's C++14). To get a promise into the worker:

```cpp
/* Option 1: capture by ref, join before scope ends. */
std::promise<int> p;
std::thread t([&p] { p.set_value(...); });
t.join();

/* Option 2: shared_ptr to the promise. */
auto p = std::make_shared<std::promise<int>>();
std::thread t([p] { p->set_value(...); });
```

The reference + join pattern is what we use here -- the
promise lives in the calling function's stack frame, and the
join guarantees the worker is done before the frame unwinds.

### `wait_for` and `wait_until`

`std::future::wait_for(duration)` and `wait_until(time_point)`
let you poll with a timeout:

```cpp
auto status = f.wait_for(std::chrono::seconds(1));
if (status == std::future_status::ready) {
    int v = f.get();
}
```

The status enum has `ready`, `timeout`, and `deferred`. Not
part of this task; useful in real timeout-aware code.

## Task

In `solution.hpp`, implement a function template:

```cpp
namespace task119
{
    /* Spawn a worker thread that computes fn(x). The worker fulfils
     * a std::promise<int>; the main thread waits on the matching
     * future and returns the value (or rethrows if fn threw). */
    template <class F>
    int async_compute_int(F fn, int x);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body shape (C++11-friendly, capture promise by ref):
  ```cpp
  std::promise<int> p;
  std::future<int>  f = p.get_future();

  std::thread worker([&p, fn, x] {
      try { p.set_value(fn(x)); }
      catch (...) { p.set_exception(std::current_exception()); }
  });

  worker.join();
  return f.get();
  ```

- `std::promise<int>` and `std::future<int>` are in
  `<future>`. Include it.

- `p.get_future()` returns the *one* future tied to `p`'s
  shared state. Calling `get_future` twice on the same promise
  throws `std::future_error`.

- `f.get()` blocks until the promise is fulfilled (value or
  exception). It moves the value out of the future, so calling
  `f.get()` twice is undefined behaviour. Check `f.valid()` if
  unsure.

- The try/catch in the worker is what makes the function
  thread-safe for throwing `fn`s. Without it, an exception in
  `fn` would propagate out of the thread, which calls
  `std::terminate` (same trap as steps 96 and 111).

- The tests pass lambdas, function pointers, and bind results
  to verify the function template's reach.

- The lambda captures `&p, fn, x`. `fn` and `x` are captured
  by value (lvalue copies into the lambda). `p` by reference,
  because:
  - It's not copyable.
  - It's also not movable into a C++11 lambda capture without
    init capture (C++14).
  - The join() before `f.get()` ensures p outlives the worker.

- C++14's lambda init-capture (`[p = std::move(p), fn, x]`)
  lets you move the promise in. The C++11 form here is the
  by-ref + join() workaround.
