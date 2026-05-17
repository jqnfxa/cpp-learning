# 112 — `std::thread::join` / `detach` + a `joiner` RAII wrapper

> Spec: <https://en.cppreference.com/cpp/thread/thread/join>,
> <https://en.cppreference.com/cpp/thread/thread/detach>

## What you learn

A `std::thread` has exactly two legal "lifecycle endings":

- **`join()`**: block until the thread finishes. After `join`,
  the `std::thread` object is no longer joinable.
- **`detach()`**: release ownership. The thread keeps running
  unsupervised; you can't interact with it any more.

If neither happens before the `std::thread` destructor fires
**while the thread is joinable**, the destructor calls
`std::terminate`. That's a hard crash; you don't recover.

```cpp
{
    std::thread t([]{ work(); });
    /* forgot to join or detach */
}   /* ~thread() called on a joinable thread -> std::terminate */
```

The fix in a single line: **always wrap `std::thread` in an
RAII type that joins in its destructor**. The standard didn't
ship one (C++20 finally adds `std::jthread`), so in C++11/14/17
you write your own.

### The `joiner` pattern

```cpp
class joiner
{
    std::thread t_;
public:
    explicit joiner(std::thread t) : t_(std::move(t)) {}

    /* No copying. */
    joiner(const joiner &)            = delete;
    joiner &operator=(const joiner &) = delete;

    /* Allow move (transfers thread ownership). */
    joiner(joiner &&)                 = default;
    joiner &operator=(joiner &&)      = delete;   /* simplifying choice */

    ~joiner()
    {
        if (t_.joinable()) t_.join();
    }

    bool                  joinable() const noexcept { return t_.joinable(); }
    std::thread::id       get_id()   const noexcept { return t_.get_id(); }
};
```

Three load-bearing details:

1. **`if (t_.joinable()) t_.join();`** — a `std::thread` after
   `join()` (or after being moved-from) is no longer joinable.
   The guard prevents "double join" from being a bug. (Calling
   `join()` on a non-joinable thread is UB.)

2. **No copy operations**. A `std::thread` represents
   *ownership* of an OS thread; copying would mean "two owners
   of the same thread", which doesn't make sense.

3. **Move construction transfers**. You can build a joiner
   from a thread, hand the joiner around, and the inner thread
   gets joined exactly once — at the destructor of the final
   owner.

Move assignment is also reasonable but requires careful
handling (join the current thread first, then take over the
new one). For this task we keep it out by `= delete`.

### Detach is the *other* option

```cpp
~joiner_d()
{
    if (t_.joinable()) t_.detach();   /* let it run unsupervised */
}
```

This prevents `std::terminate` but doesn't *wait*. After the
destructor returns, the thread is still running, with no way
to tell when it finishes. Resources it touches (captured
variables, file descriptors, RAII handles) **must outlive the
thread**, which usually means leaking them or referring only
to global state.

For "fire-and-forget logging" or "background reaper" daemons,
detach is fine. For most code, **join is the right answer**.

### Why this matters

The classic bug:

```cpp
void serve_request()
{
    std::thread worker(do_work);
    if (something) return;            /* ~thread() on joinable -> terminate */
    worker.join();
}
```

The early return path forgets to join. `std::terminate` fires.
The fix:

```cpp
void serve_request()
{
    joiner j(std::thread(do_work));
    if (something) return;            /* ~joiner() joins -> safe */
}
```

One letter different at the declaration, one whole class of bug
gone. This is exactly the RAII trade — push lifetime management
into a type's destructor and the call site stops being
error-prone.

## Task

In `solution.hpp`, complete the `joiner` class so that:

- Constructible from `std::thread &&`.
- Non-copyable.
- Move-constructible (default is fine).
- Move-assignment deleted.
- Destructor **joins** the thread if it's joinable. (The
  scaffold ships with `detach()` in the destructor — the
  TODO is to swap it for `join`.)
- `joinable()` and `get_id()` forward to the inner thread.

## Run

```sh
./verify.sh
```

## Hints

- The destructor body is one conditional line:
  ```cpp
  if (t_.joinable()) t_.join();
  ```

- The `if (t_.joinable())` guard is **not** optional. Without
  it, a moved-from `joiner` (whose inner thread is no longer
  joinable) would crash on destruction. With it, the
  moved-from case is a no-op.

- For the tests, the "destructor blocks until the thread
  finishes" check uses a `std::shared_ptr<std::atomic<bool>>`
  shared between the test code and the thread. The shared_ptr
  ensures the storage outlives even a detached thread, so the
  stub's wrong behaviour (detach instead of join) doesn't crash
  the test process -- it just fails the assertion.

- Move ctor `= default` works because `std::thread` is itself
  move-constructible. The default destructor of the moved-from
  joiner sees a non-joinable thread and skips the join thanks
  to the guard.

- We delete move-assignment to keep the class small. A full
  move-assign would have to `if (t_.joinable()) t_.join();`
  first, then move the new thread in. C++20's `std::jthread`
  does exactly that; for this task, deletion is fine.

- C++20's `std::jthread` *is* this class, plus
  `std::stop_token` for cooperative cancellation. Knowing the
  C++11/14/17 hand-rolled version makes `jthread` look like an
  obvious next step rather than magic.
