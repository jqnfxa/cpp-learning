# 111 — `std::thread`

> Spec: <https://en.cppreference.com/cpp/thread/thread>

## What you learn

`std::thread` is C++11's portable threading primitive. A
constructed-with-a-callable `std::thread` starts running that
callable on a new OS thread immediately:

```cpp
std::thread t([] { do_work(); });
/* `t` is now running do_work() on a separate thread. */
t.join();      /* block until it finishes */
```

The minimal lifecycle:

1. **Constructor takes a callable.** Lambda, function pointer,
   functor, `std::bind` result -- anything callable.
2. **Optionally extra args.** They're forwarded to the
   callable: `std::thread t(f, 1, 2, "hi");` calls
   `f(1, 2, "hi")`.
3. **Must be `join()`ed or `detach()`ed before it goes out of
   scope.** Otherwise the destructor calls `std::terminate`.
4. **`get_id()`** returns the thread's `std::thread::id`, a
   comparable opaque handle.

### Thread IDs

Each running thread has a unique `std::thread::id`:

```cpp
std::thread::id main_id = std::this_thread::get_id();

std::thread t([&] {
    std::thread::id worker_id = std::this_thread::get_id();
    /* worker_id != main_id */
});
t.join();
```

A default-constructed `std::thread::id` is the "no thread"
sentinel; running threads have a non-default id. IDs are
comparable (`==`, `!=`), hashable (`std::hash`), and printable
(`operator<<` to streams). They're *not* portably mappable to
OS thread IDs -- the standard says only "uniquely identifies a
thread while it's running".

### The `join()`-or-`detach()` rule

A `std::thread` whose destructor runs while the object is still
"joinable" (i.e. has an associated running thread that hasn't
been joined or detached) calls `std::terminate`. The two
options:

- **`t.join()`**: block until `t` finishes; then `t` is no
  longer joinable.
- **`t.detach()`**: forfeit ownership; the thread keeps
  running, but you can no longer interact with it.

For most code, **join is the right answer**. Detach is for
"fire-and-forget" daemons; in practice almost always a bad
idea because the detached thread can outlive the resources it
captured.

(Step 112 covers `join` / `detach` and the RAII `joiner`
wrapper that automates this.)

### Constructor accepts forwarded args

```cpp
void f(int n, const std::string &name);

std::thread t(f, 42, std::string("alice"));
/* equivalent to: f(42, std::string("alice"))   on a new thread */
```

Args are forwarded via `std::forward`. Rvalues move, lvalues
copy. The standard's wording: arguments are "decay-copied"
into the thread's internal storage before invocation, so
references *don't* propagate by default -- you need
`std::ref` / `std::cref` for that:

```cpp
int counter = 0;
std::thread t([&] { ++counter; });   /* ok, lambda captures by ref */

std::thread t2(some_fn, counter);     /* COPY of counter into the thread */
std::thread t3(some_fn, std::ref(counter));  /* reference passed */
```

Capturing by reference in a lambda is the path most code
takes; `std::ref` shows up mostly in C-API interop.

### Sharing data between threads

`std::thread` itself only starts the thread and lets you join
it. Synchronisation -- protecting shared data, signaling --
needs `std::mutex` (step 113), `std::atomic` (step 116), or
higher-level primitives like `std::async` (not in this
section's tasks).

## Task

In `solution.hpp`, implement one function:

```cpp
namespace task111
{
    /* Spawn a worker thread that captures its own std::thread::id,
     * join the thread, and return the captured id. */
    std::thread::id run_and_get_thread_id();
}
```

## Run

```sh
./verify.sh
```

## Hints

- The four-line shape:
  ```cpp
  std::thread::id captured;
  std::thread t([&] { captured = std::this_thread::get_id(); });
  t.join();
  return captured;
  ```

- The lambda captures `captured` by reference so it can write
  to it. `[&]` captures everything by reference; `[&captured]`
  is more precise.

- `t.join()` *must* run before the function returns. If
  `std::thread`'s destructor fires while it's still joinable,
  `std::terminate` is called -- the test would not return a
  value, the harness would report a crashed process.

- `std::this_thread::get_id()` is the call-from-inside-this-thread
  way to ask "who am I?". It's a free function in the
  `std::this_thread` sub-namespace, alongside
  `sleep_for` (step 108's tests) and `sleep_until`.

- Tests use `EXPECT_NE` to compare the captured worker id
  against the main thread's id. They must differ -- if they
  don't, the lambda ran on the calling thread, which means
  `std::thread` did the wrong thing.

- `std::thread::id` doesn't have an `int` conversion, but it's
  printable with `<<` if you want diagnostics. Tests use
  equality comparison directly.

- C++20 added `std::jthread` -- a thread that auto-joins in its
  destructor and supports cooperative cancellation via
  `std::stop_token`. For C++11 you write the auto-join wrapper
  yourself (step 112).
