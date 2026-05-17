# 116 — `std::condition_variable`

> Spec: <https://en.cppreference.com/cpp/thread/condition_variable>

## What you learn

A **condition variable** lets a thread *wait until another
thread signals*. The pattern is the classic producer/consumer:

- A thread that wants something blocks on the cv.
- A thread that has something to share signals (notifies) the
  cv, waking one (or all) of the waiters.

C++11's `std::condition_variable` works in tandem with a
`std::mutex` and a `std::unique_lock`. The full
`wait + condition + notify` shape:

```cpp
std::mutex m;
std::condition_variable cv;
bool ready = false;

/* waiting thread */
std::unique_lock<std::mutex> lk(m);
cv.wait(lk, [&] { return ready; });   /* atomic: unlock, wait, re-lock */
/* `ready` is now true; lock is held */

/* signalling thread */
{
    std::lock_guard<std::mutex> g(m);
    ready = true;
}
cv.notify_one();
```

`cv.wait(lk, pred)` does three things atomically:
1. If `pred()` is already true, returns immediately.
2. Otherwise: unlocks `lk`, blocks on the cv, and *re-locks*
   `lk` when notified.
3. After being notified, re-checks `pred()` and goes back to
   step 2 if it's still false (this is the "spurious wakeup"
   guard).

The two-arg form (with predicate) is the form you should
almost always use. The bare `cv.wait(lk)` form requires you to
re-check the condition manually after the wake -- easy to get
wrong.

### The bounded-buffer pattern

The textbook use of condition_variable is **bounded buffer**:
a queue with a capacity that blocks producers when full and
consumers when empty.

```cpp
class bounded_buffer
{
    std::deque<int>           q_;
    std::size_t               cap_;
    mutable std::mutex        m_;
    std::condition_variable   not_full_;
    std::condition_variable   not_empty_;

public:
    explicit bounded_buffer(std::size_t cap) : cap_(cap) {}

    void push(int v)
    {
        std::unique_lock<std::mutex> lk(m_);
        not_full_.wait(lk, [&] { return q_.size() < cap_; });
        q_.push_back(v);
        not_empty_.notify_one();
    }

    int pop()
    {
        std::unique_lock<std::mutex> lk(m_);
        not_empty_.wait(lk, [&] { return !q_.empty(); });
        int v = q_.front();
        q_.pop_front();
        not_full_.notify_one();
        return v;
    }
};
```

Two condition variables, one for each side:
- `not_full_`: producers wait here; consumers signal here after
  popping (the buffer became less full).
- `not_empty_`: consumers wait here; producers signal here after
  pushing (the buffer became less empty).

Both `push` and `pop` are blocking. With a small capacity and a
fast producer, `push` will *wait*. With an empty buffer and an
eager consumer, `pop` will *wait*. The wait correctly releases
the mutex so the other side can make progress.

### `notify_one` vs `notify_all`

- `notify_one`: wake exactly one waiter (any waiter; impl-
  defined which).
- `notify_all`: wake every waiter; they all re-acquire the
  mutex one at a time.

For single-producer-single-consumer, `notify_one` is enough --
there's at most one waiter on each side. For broadcast events
(e.g., shutdown signal, "data is ready" with multiple consumers
that each want a copy), `notify_all` is the right tool.

A common mistake: `notify_all` "to be safe". It wastes CPU when
only one thread needs to be woken. Use `notify_one` unless the
design needs broadcast.

### Spurious wakeups

The C++ standard explicitly allows `cv.wait()` to return
without a `notify_*` call. This is the "spurious wakeup":
spurious because no signal was sent. They're rare in practice
on Linux/glibc but allowed by the standard for implementation
flexibility.

The two-arg `wait(lk, pred)` handles spurious wakeups
automatically by re-checking the predicate. The bare `wait(lk)`
form does not; with it, you must wrap in a manual `while (!pred)
wait(lk);` loop. Always prefer the two-arg form.

## Task

In `solution.hpp`, complete the `bounded_buffer` class so that:

- `push(v)` blocks when the buffer is at capacity; wakes when
  space frees up.
- `pop()` blocks when the buffer is empty; wakes when an item
  arrives. Returns the popped value.
- `size()` returns the current count (protected by the lock).

The scaffold ships with non-blocking `push` (allows overflow)
and `pop` (returns -1 sentinel on empty). The TODO is to add
the proper `wait` calls and `notify_one` signals.

## Run

```sh
./verify.sh
```

## Hints

- `push` body:
  ```cpp
  std::unique_lock<std::mutex> lk(m_);
  not_full_.wait(lk, [&] { return q_.size() < cap_; });
  q_.push_back(v);
  not_empty_.notify_one();
  ```

- `pop` body:
  ```cpp
  std::unique_lock<std::mutex> lk(m_);
  not_empty_.wait(lk, [&] { return !q_.empty(); });
  int v = q_.front();
  q_.pop_front();
  not_full_.notify_one();
  return v;
  ```

- The predicate captures `this`/the queue by reference (via
  `[&]`). Each spurious wakeup re-evaluates it. Cheap.

- `wait` takes `unique_lock`, not `lock_guard` -- because
  `wait` needs to *unlock* the mutex during the wait. That's
  the headline `unique_lock` feature from step 114.

- `notify_one` doesn't require the lock to be held; you can
  notify after unlocking. In practice, holding the lock during
  notify is fine and slightly simpler. The Linux pthread
  implementation handles both cases.

- Tests use a single-producer single-consumer pair. The
  consumer is spawned *first*, with a small delay before the
  producer starts. With proper blocking, the consumer waits.
  With the stub (no waits), the consumer's `pop` returns -1
  sentinel during the delay, ruining the output.

- The two-cv pattern (not_full_, not_empty_) is standard. A
  single cv with two waiters works too but is less precise --
  every notify wakes both sides, wasting CPU on the wrong
  side.

- C++14 added `std::shared_lock` for reader-writer scenarios.
  C++20 added `std::counting_semaphore` and `std::latch` /
  `std::barrier` for more direct signaling. For "bounded
  buffer between two threads", the cv + mutex pair from C++11
  is still the textbook answer.
