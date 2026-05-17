# 96 — `std::exception_ptr`, `current_exception`, `rethrow_exception`

> Spec: <https://en.cppreference.com/cpp/error/exception_ptr>

## What you learn

`std::exception_ptr` is a **smart-pointer-shaped handle to a
caught exception**. Once you've got one in a `catch (...)`
block, you can:

- Store it in a variable, a member, a container.
- Move it across thread boundaries.
- Re-throw it later (possibly on a different thread)
  with `std::rethrow_exception`.

Three pieces work together (all in `<exception>`):

```cpp
std::exception_ptr eptr;

try { something_dangerous(); }
catch (...) { eptr = std::current_exception(); }   /* capture */

if (eptr) std::rethrow_exception(eptr);            /* replay */
```

`std::current_exception()` may only be called from inside a
catch block (or it returns an empty `exception_ptr`).
`std::rethrow_exception()` re-throws on the *current* thread,
typed as it was when caught — so a `catch (std::runtime_error
&)` on the rethrow side will still match.

### Why this exists

C++ exceptions are *thread-local*. If a worker thread throws,
the main thread can't catch it -- the unwind path stops at the
thread boundary (and, in fact, calls `std::terminate` if the
thread function's exception isn't caught locally). To get
exceptions across threads you need to *materialise* them into
a value, transfer the value, and *re-throw* on the other side.

`std::exception_ptr` is exactly that value type. The trio
forms the standard exception-marshalling protocol:

```
worker thread                         main thread
─────────────                         ───────────
try { ... } catch (...) {
    eptr = current_exception();   ─►  /* eptr lives in shared state */
}
                                      if (eptr)
                                          rethrow_exception(eptr); /* throws here */
```

### Type-preservation

`rethrow_exception(eptr)` rethrows the exception **with its
original dynamic type**. Inside a `try` on the main thread,
`catch (std::runtime_error &e)` will catch what was originally a
`std::runtime_error` on the worker. Catch hierarchies survive
the round-trip.

```cpp
try { run_and_propagate([]{ throw std::out_of_range("..."); }); }
catch (const std::out_of_range &) { /* catches */ }
catch (const std::exception &)    { /* skipped */ }
```

### How `std::future` uses this

`std::future`'s "what happens if the task throws?" semantics
are exactly this protocol. The promise side stores the
exception via `std::promise::set_exception(std::exception_ptr)`,
and the future side rethrows it on `future::get()`. The same
machinery powers `std::async`.

### Limitations

- **Cannot inspect the held exception** without rethrowing it.
  `std::exception_ptr` is opaque; the only operations are
  copy/move, comparison to nullptr, and rethrow.
- **Comparison only against `nullptr`**, not equality of
  exceptions.
- **The exception object's lifetime is shared** -- the
  reference-counted `exception_ptr` keeps it alive until the
  last copy is destroyed.

C++14 added `std::make_exception_ptr<E>(E)` which is the
"craft an exception_ptr without throwing first" helper.

## Task

In `solution.hpp`, implement a function template that runs a
callable on a worker thread and propagates any exception to the
main thread:

```cpp
namespace task96
{
    /* Run fn() on a new thread. If fn() throws, capture the
     * exception with std::current_exception, join the thread, then
     * std::rethrow_exception on this thread. If fn() returns
     * normally, this function returns normally too. */
    template <class F>
    void run_and_propagate(F fn);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body is the canonical pattern:
  ```cpp
  std::exception_ptr eptr;
  std::thread t([&] {
      try { fn(); }
      catch (...) { eptr = std::current_exception(); }
  });
  t.join();
  if (eptr) std::rethrow_exception(eptr);
  ```

- Capture `eptr` and `fn` by reference in the lambda. The thread
  outlives the call only briefly; `t.join()` synchronises before
  we touch `eptr`.

- `t.join()` *must* happen before `rethrow_exception`. If you
  rethrow first, the thread is still running (or detached
  improperly), and you've got either a use-after-free or a
  thread-termination hazard.

- Don't `throw eptr` directly. `rethrow_exception` takes an
  `exception_ptr` by value and re-throws the held object.
  Throwing the `exception_ptr` itself would just be throwing a
  smart pointer.

- A thread function's *direct* exception (uncaught at thread
  scope) calls `std::terminate`. The `try/catch (...)` wrapper
  inside the lambda is what prevents that. Without the catch,
  `t.join()` is fine, but the program is already dead.

- The function template lives in the header. Function templates
  are implicitly inline; no need for explicit `inline`.

- Pair this with `std::future` + `std::async` in a real
  codebase: those provide the same exception-marshalling for
  free, with timing and value-return on top.
