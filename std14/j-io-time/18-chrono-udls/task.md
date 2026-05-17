# 18 — chrono UDLs (`s`, `ms`, `us`, `ns`, `min`, `h`)

> Spec: <https://en.cppreference.com/cpp/chrono/operator%22%22s>

## What you learn

C++14 added user-defined literals (UDLs) for the
`std::chrono` duration types. They live in
`std::chrono_literals` and let you write durations
naturally:

```cpp
using namespace std::chrono_literals;

50ms          /* std::chrono::milliseconds(50)        */
2s            /* std::chrono::seconds(2)              */
500ns         /* std::chrono::nanoseconds(500)        */
1min          /* std::chrono::minutes(1)              */
24h           /* std::chrono::hours(24)               */
1us           /* std::chrono::microseconds(1)         */
```

Before this, you wrote:

```cpp
std::chrono::milliseconds(50)
std::chrono::seconds(2)
```

Verbose, especially when these appear inline (timeouts,
sleeps, scheduling).

### The six suffixes

| UDL    | Type                            | Equivalent          |
|--------|---------------------------------|---------------------|
| `1ns`  | `std::chrono::nanoseconds(1)`   | 1 nanosecond        |
| `1us`  | `std::chrono::microseconds(1)`  | 1 microsecond       |
| `1ms`  | `std::chrono::milliseconds(1)`  | 1 millisecond       |
| `1s`   | `std::chrono::seconds(1)`       | 1 second            |
| `1min` | `std::chrono::minutes(1)`       | 1 minute            |
| `1h`   | `std::chrono::hours(1)`         | 1 hour              |

Each UDL exists in two forms: integer and floating-point:

```cpp
2.5s     /* std::chrono::duration<double>(2.5)  -- 2.5 seconds */
2s       /* std::chrono::seconds(2)             -- exact int */
```

The floating-point form is a `duration<double, ratio>`, not
the regular integer duration type. They're convertible but
distinct types.

### The `chrono_literals` namespace

The UDLs are in two nested namespaces:

```cpp
namespace std::chrono_literals   /* the inline namespace within chrono */

namespace std::literals          /* the global "literals" namespace */
namespace std::literals::chrono_literals   /* same UDLs */
```

To use them:

```cpp
using namespace std::chrono_literals;       /* OR */
using namespace std::literals;              /* OR */
using namespace std::literals::chrono_literals;
```

All three import the same UDL operators. The recommended
form (per the standard) is `using namespace
std::chrono_literals;` -- minimal namespace pollution, just
the chrono UDLs.

### Where you'll use them

The textbook use is in `std::this_thread::sleep_for`:

```cpp
std::this_thread::sleep_for(50ms);
std::this_thread::sleep_for(2s);
```

Also in:
- `std::condition_variable::wait_for(lk, 100ms, predicate)`
- `std::future::wait_for(500us)`
- `std::async` -- doesn't take a duration but uses one
  internally.
- Timer expressions: `auto timeout = now + 30s;`

### Type deduction

```cpp
auto a = 50ms;           /* std::chrono::milliseconds */
auto b = 2.5s;           /* std::chrono::duration<double>      */
auto c = 1min + 30s;     /* common_type = std::chrono::seconds  */
```

The mixed-unit arithmetic in `c` uses `common_type` to find
the unit that exactly represents both. Different time scales
combine cleanly:

```cpp
1h + 30min + 45s + 100ms   /* milliseconds, no precision loss */
```

### Implicit narrowing

Integer divisions on durations are well-defined and lossy:

```cpp
auto x = 1500ms;
auto y = std::chrono::duration_cast<std::chrono::seconds>(x);
/* y == 1s -- truncated */
```

The implicit conversion only happens between losslessly-
convertible units:

```cpp
std::chrono::milliseconds m = 2s;     /* OK: 2s -> 2000ms exact */
std::chrono::seconds s = 1500ms;      /* error: lossy */
```

Use `duration_cast` when you intend the narrowing.

### Why not in the global namespace?

C++ standard library reserves user-suffix names starting
with `_`. The chrono UDLs use plain letter suffixes
(`s`, `ms`, etc.) -- safe to expose only because they're in
a namespace that the user has to opt in to.

If they were in the global namespace, every program that
used `using namespace std::chrono` would have to deal with
`1s` being interpreted as a chrono literal even when the
programmer didn't intend it. The namespace gate prevents
that.

### Conflict: `s` is also a string UDL

C++14 added `s` as a string literal UDL too:
`"hello"s` -> `std::string("hello")`.

The two are differentiated by context: `1s` is a numeric
literal (chrono duration), `"x"s` is a string literal
(std::string). The same suffix means different things to
different prefixes; the language disambiguates by the
literal's grammar.

You can use both in the same TU:
```cpp
using namespace std::literals;
auto t = 1s;             /* chrono::seconds */
auto str = "hello"s;     /* std::string */
```

The `using namespace std::literals` brings in BOTH the
chrono and string UDLs (and `complex` too). Use
`std::chrono_literals` specifically if you only want
durations.

### constexpr

Chrono UDLs are constexpr:

```cpp
constexpr auto half_minute = 30s;     /* constexpr-evaluated */
constexpr auto two_hours = 2h;
```

Useful for compile-time durations (e.g., default timeouts in
constexpr config classes).

## Task

In `solution.hpp`, set six constants to the correct UDL
expressions:

```cpp
using namespace std::chrono_literals;

constexpr auto fifty_ms         = 50ms;
constexpr auto two_seconds      = 2s;
constexpr auto one_minute       = 1min;
constexpr auto one_day          = 24h;
constexpr auto five_hundred_ns  = 500ns;

constexpr auto hundred_ms() { return 100ms; }
```

The scaffold has each constant set to the WRONG unit (off by
a factor of 1000 or wrong scale). Fix them.

## Run

```sh
./verify.sh
```

## Hints

- `<chrono>` is the header. Already included.

- `using namespace std::chrono_literals;` at namespace
  scope inside `task18` brings the UDLs into the namespace
  so the constants compile.

- `50ms` and `50us` look similar but differ by 1000 (ms = milli,
  us = micro). Pay attention to the prefix.

- `1min` is the minutes suffix (`min`, not `m` -- `m` would
  conflict with meters in `<ratio>`).

- `24h` is the typical way to write a day (since C++ doesn't
  have a `day` UDL; C++20 added `chrono::days` and a `d`
  suffix).

- `500ns` is 500 nanoseconds. The stub's `500ms` is 500
  MILLI-seconds (a million times longer).

- The `hundred_ms()` function is a small wrapper used in the
  type-check test -- it must return `std::chrono::milliseconds`
  (the natural type of `100ms`).

- Tests verify: each constant equals its
  std::chrono::duration form; conversions across units;
  type-check via runtime is_same<decltype(fifty_ms), const
  std::chrono::milliseconds>; constexpr-evaluable.

- The "type is milliseconds" check is the deeper proof: it
  pins that `50ms` deduces to `std::chrono::milliseconds`
  specifically, not some general `duration<int, milli>` or
  the wrong unit.

- C++20 added a `d` suffix for `chrono::days` and `y` for
  `chrono::years` -- not in C++14. Use `24h` for a day if you
  need C++14 compatibility.

- These UDLs are constexpr. You can use them in constant
  expressions, template arguments, array sizes (when wrapped
  in `duration_cast<int>(d).count()`).
