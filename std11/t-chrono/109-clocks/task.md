# 109 — clocks: `system_clock`, `steady_clock`, `high_resolution_clock`

> Spec: <https://en.cppreference.com/cpp/chrono#Clocks>

## What you learn

C++11's `<chrono>` ships three clocks. Each represents a
different notion of "now":

| Clock                          | Epoch / kind         | Can it go backward? | Right for                |
|--------------------------------|----------------------|----------------------|--------------------------|
| `std::chrono::system_clock`     | Wall clock (Unix epoch) | **Yes** -- NTP, manual setting | "What time is it on the wall?" |
| `std::chrono::steady_clock`     | Boot or arbitrary monotonic | **No** | Measuring durations, timeouts |
| `std::chrono::high_resolution_clock` | Aliased to one of the above | depends | Don't use (see below) |

### `system_clock` — the wall clock

```cpp
auto now = std::chrono::system_clock::now();
auto t   = std::chrono::system_clock::to_time_t(now);
/* t is a Unix time_t -- pass to ctime/localtime for printing */
```

Use this for:
- Timestamps in logs ("the event happened at 2026-05-17 12:34").
- Scheduling against a calendar date.
- Anything that needs to be human-readable in clock terms.

The catch: it can **jump**. NTP synchronisation can move it
forward or backward by seconds. Manual administrative time
changes are even more dramatic. **Do not use it for measuring
elapsed time** — your "elapsed" can come out negative.

### `steady_clock` — the monotonic clock

```cpp
auto start = std::chrono::steady_clock::now();
do_work();
auto end   = std::chrono::steady_clock::now();
auto elapsed = end - start;   /* never negative */
```

The guarantee: `t2 >= t1` if `t2` was sampled after `t1`. No
jumps, no NTP shenanigans, no DST. Step 108's `measure` helper
uses this clock for exactly this reason.

Use this for:
- Profiling / benchmarking (how long did this take?).
- Timeouts (`wait_until(start + 10s)` -- starts behave correctly
  even if wall clock jumps).
- Any rate-limit or backoff logic.

It has **no** mapping to wall-clock time. You can't ask
"steady_clock at 12:34 PM"; the epoch is unspecified
(implementation-defined, typically system boot).

### `high_resolution_clock` — usually a trap

The standard says it should be "the clock with the shortest
tick period available on the platform." In practice:

- **libstdc++**: alias for `system_clock`.
- **libc++**: alias for `steady_clock`.
- **MSVC**: alias for `steady_clock`.

That means writing `high_resolution_clock::now() - ...::now()`
gives you `system_clock`'s "can go backward" behaviour on
libstdc++ and `steady_clock`'s monotonicity elsewhere. The
portability is in your imagination.

**Recommendation**: don't use `high_resolution_clock`. Pick
`steady_clock` for measurement (always works), `system_clock`
for wall time (always works). The "auto-pick the best
resolution" promise just isn't real.

### The three scenarios in this task

1. **Timeout** ("wait at most 10 seconds for the lock"): you
   want a deadline that won't get cancelled by a wall-clock
   jump -> `steady_clock`.

2. **Profiling** ("how long did this function take?"):
   monotonic, never negative -> `steady_clock`.

3. **Current wall time** ("log this event at 2026-05-17
   12:34:56"): wall clock, the human-readable one ->
   `system_clock`.

In real codebases, this pattern shows up so often that
"profile / timeout = steady; log = system" is a fixed mental
mapping experienced C++ programmers carry around.

## Task

In `solution.hpp`, fill in three constants:

```cpp
namespace task109
{
    /* Set each to "steady_clock" or "system_clock". */
    constexpr const char *clock_for_timeout    = "TODO";
    constexpr const char *clock_for_profiling  = "TODO";
    constexpr const char *clock_for_wall_time  = "TODO";
}
```

The values are spell-by-name; tests compare with `std::strcmp`.

## Run

```sh
./verify.sh
```

## Hints

- Two of the three answers are `"steady_clock"`; one is
  `"system_clock"`. None is `"high_resolution_clock"`.

- `constexpr const char *` is the C++11 way to name a compile-
  time string. The value lives in static storage; reading
  `clock_for_timeout` returns a pointer to it.

- This is a classification task -- like step 66
  (memory_model_conceptual). The point is the *decision*, not
  the syntax.

- The "right answer" follows from the table above: if the
  scenario involves a deadline or elapsed-time measurement,
  use `steady_clock`. If the scenario involves human-readable
  wall time, use `system_clock`.

- A real codebase might encode this as
  `template <class Use> using clock_for = ...;` with a
  trait-based dispatch, but for learning the string-keyed form
  is plenty.

- `high_resolution_clock` shows up in interview questions and
  StackOverflow answers more than in production C++; if you
  see it in real code, it's usually a mistake or a leftover
  from a less-informed era.
