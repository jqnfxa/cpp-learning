# 108 — `std::chrono::time_point`

> Spec: <https://en.cppreference.com/cpp/chrono/time_point>

## What you learn

A `std::chrono::time_point<Clock, Duration>` represents *a
moment in time*, the way a duration represents *a time span*.
It's defined as "a duration since some epoch", where the epoch
is the clock's own convention:

```cpp
auto t = std::chrono::steady_clock::now();
/* t : std::chrono::time_point<std::chrono::steady_clock, ...> */
```

### The two kinds of operations

| Expression                                    | Result      |
|-----------------------------------------------|-------------|
| `time_point + duration`                       | `time_point` |
| `time_point - duration`                       | `time_point` |
| `time_point - time_point` (same clock)        | `duration` (!) |

Subtracting two time points of the same clock gives a
**duration**. That's the headline operation for measurement:

```cpp
auto start = std::chrono::steady_clock::now();
do_work();
auto end = std::chrono::steady_clock::now();
auto elapsed = end - start;     /* a duration */
```

Adding/subtracting *time points* of *different* clocks does
not compile. The clock identity is part of the time_point's
type. That prevents "compared a system_clock to a steady_clock"
class of bugs.

### Picking a clock

For *measuring elapsed time* you want
`std::chrono::steady_clock`. Its guarantee is that the clock
never goes backwards -- even if the system administrator
adjusts the wall clock during your measurement.
`system_clock` is the wall clock; it CAN jump (NTP adjustments,
manual time changes). For benchmarks and timeouts, `steady_clock`
is the correct primitive.

(Step 109 covers all three clocks in depth: system_clock,
steady_clock, high_resolution_clock.)

### Casting the elapsed duration

`steady_clock::now() - steady_clock::now()` produces a duration
in the clock's native period (often nanoseconds on Linux).
You'll usually want to coerce to a human-friendly unit:

```cpp
auto us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
```

`duration_cast` (step 107) does the truncation. For "print the
elapsed microseconds", that's the call.

### The classic measure-a-function helper

The pattern shows up so often it has a name -- the
**`measure`** helper:

```cpp
template <class F>
std::chrono::microseconds measure(F fn)
{
    auto start = std::chrono::steady_clock::now();
    fn();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}
```

`measure([]{ do_work(); })` returns the elapsed microseconds.
This is the C++11 equivalent of `time { do_work }` in shell.

### Caveats for accurate measurement

- **Compiler may optimise out the lambda body** if its result
  is unused. To prevent that, return a value from the lambda or
  use a global `volatile` sink.
- **First iteration is slower** (warm-up, branch prediction,
  caches). For real benchmarks, run multiple times and take
  the minimum.
- **Threading / OS scheduling** introduces jitter. For sub-
  microsecond measurements, average over thousands of samples.

For this task we ignore those -- the test verifies the helper
works, not that it produces lab-grade timings.

## Task

In `solution.hpp`, implement one function template:

```cpp
namespace task108
{
    /* Run fn() once. Return the elapsed time in microseconds, as
     * measured by std::chrono::steady_clock. */
    template <class F>
    std::chrono::microseconds measure(F fn);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body is the four-line pattern from above:
  ```cpp
  auto start = std::chrono::steady_clock::now();
  fn();
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  ```

- The function template lives in the header. No explicit
  `inline` required.

- Use `std::chrono::steady_clock`, NOT `system_clock`, for the
  measurement. `system_clock::now() - system_clock::now()` can
  be *negative* if the wall clock jumped backwards mid-test --
  that's the whole reason steady_clock exists.

- The result type is `std::chrono::microseconds`. `duration_cast`
  takes the clock's native duration (often nanoseconds) and
  truncates to microseconds.

- Tests use `std::this_thread::sleep_for(...)` to inject a
  controlled delay. The measured elapsed must be at least that
  much, possibly more (scheduling slop). Tests check the lower
  bound, not exact equality.

- `std::this_thread::sleep_for` is in `<thread>`. Include it in
  the test (not the solution -- the solution doesn't need it).

- A more elaborate version of the helper would return the
  callable's return value alongside the duration, often as a
  `std::pair<R, microseconds>` or via separate "ran" /
  "duration" out-params. For this task, just the duration is
  enough.
