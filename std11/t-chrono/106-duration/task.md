# 106 — `std::chrono::duration`

> Spec: <https://en.cppreference.com/cpp/chrono/duration>

## What you learn

`std::chrono::duration<Rep, Period>` is C++11's **typed time
span**. It represents "how long" -- an interval, not a point in
time. The type encodes:

- `Rep`: the underlying arithmetic representation (e.g. `int`,
  `long long`, `double`).
- `Period`: a `std::ratio` describing the tick size in seconds
  (e.g. `std::ratio<1>` = seconds, `std::ratio<1, 1000>` =
  milliseconds, `std::ratio<60>` = minutes).

The standard library pre-instantiates the common ones:

```cpp
std::chrono::nanoseconds   /* duration<int64_t, std::nano>   */
std::chrono::microseconds  /* duration<int64_t, std::micro>  */
std::chrono::milliseconds  /* duration<int64_t, std::milli>  */
std::chrono::seconds        /* duration<int64_t, std::ratio<1>>     */
std::chrono::minutes        /* duration<int64_t, std::ratio<60>>    */
std::chrono::hours          /* duration<int64_t, std::ratio<3600>>  */
```

Each is a distinct type. You construct them with an integer
count:

```cpp
std::chrono::seconds       five(5);          /* 5 seconds */
std::chrono::milliseconds  hundred(100);      /* 100 milliseconds */
```

### Why typed time spans?

The headline win: the type system catches unit confusion at
compile time. You can't accidentally add nanoseconds to hours
without going through a conversion. Functions that take
`std::chrono::milliseconds` simply cannot be called with a raw
integer -- you have to spell the unit.

```cpp
void retry_after(std::chrono::milliseconds);

retry_after(100);             /* COMPILE ERROR -- raw int isn't a duration */
retry_after(std::chrono::milliseconds(100));   /* OK */
```

This single design choice prevents the "is that microseconds or
milliseconds?" class of bugs that plagues C codebases.

### `count()`

Every duration has `.count()` returning its `Rep` value:

```cpp
auto d = std::chrono::seconds(42);
d.count();   /* 42 (int64_t) */
```

### Implicit conversions between units

Conversions that *don't* lose information happen
**implicitly**:

```cpp
std::chrono::milliseconds ms = std::chrono::seconds(2);  /* OK: ms == 2000 */
```

Conversions that *might* lose information (e.g. milliseconds
-> seconds, which truncates) require explicit
`std::chrono::duration_cast` (step 107 covers this).

### Arithmetic

Durations support `+`, `-`, `*` (by Rep), `/` (by Rep or by
another duration), `%`, and comparison. Mixed-unit arithmetic
picks the *finer* unit:

```cpp
auto a = std::chrono::seconds(2) + std::chrono::milliseconds(500);
/* a is milliseconds, value 2500 */
```

This is the "common-type" rule from step 105, applied to
durations.

### C++14 literal suffixes

C++14 added `std::chrono_literals`:

```cpp
using namespace std::chrono_literals;
auto five = 5s;        /* std::chrono::seconds(5) */
auto hundred = 100ms;  /* std::chrono::milliseconds(100) */
```

C++11 doesn't have these -- you write the type and value
explicitly. The pedagogical point: knowing the long form makes
the C++14 sugar obvious when you meet it.

## Task

In `solution.hpp`, implement four functions:

```cpp
namespace task106
{
    /* Return std::chrono::seconds(5). */
    std::chrono::seconds five_seconds();

    /* Return std::chrono::milliseconds(100). */
    std::chrono::milliseconds hundred_milliseconds();

    /* Convert a seconds value to its underlying count
     * (Rep, typically long long). */
    long long count_of(std::chrono::seconds s);

    /* Add a seconds and a milliseconds, return the result as
     * milliseconds. */
    std::chrono::milliseconds add(std::chrono::seconds s, std::chrono::milliseconds ms);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `five_seconds()` is one line:
  `return std::chrono::seconds(5);` or `return std::chrono::seconds{5};`.

- `hundred_milliseconds()` is the same shape with
  `std::chrono::milliseconds(100)`.

- `count_of` is `return s.count();`. The `count()` member
  returns `Rep`, which for `seconds` is typically `long long`
  (or `int64_t`); the standard guarantees at least `int_least35`
  for second-and-above ticks.

- `add(s, ms)` is `return s + ms;`. The compiler picks
  milliseconds (the finer unit) as the result type, then
  promotes `s` into ms via the implicit no-loss conversion
  before adding. Returning as `std::chrono::milliseconds` is
  fine; the result is already in that type.

- Include `<chrono>`.

- For tests, you can compare two durations with `==` directly:
  `EXPECT_EQ(task106::five_seconds(), std::chrono::seconds(5));`.

- `std::chrono::milliseconds(100).count()` is `100`. The unit
  doesn't change the count -- the count is in units of the
  duration's *period*. 100 milliseconds is `count() == 100`,
  not `100000` microseconds.

- C++20 `std::format` has chrono-aware formatters that print
  durations with units. For C++11 you concatenate
  `std::to_string(d.count())` plus a unit suffix yourself.
