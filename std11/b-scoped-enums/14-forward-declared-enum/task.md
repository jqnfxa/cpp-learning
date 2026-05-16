# 14 — Forward-declared enums

> Spec: <https://en.cppreference.com/cpp/language/enum>

## What you learn

C++11 lets you **forward-declare** a scoped enum as long as its
underlying type is known. The classic shape:

```cpp
/* header: enum_state.hpp */
enum class state : std::uint8_t;        /* forward declaration -- no body */

state make_idle();
bool  is_terminal(state s);
```

```cpp
/* source: enum_state.cpp */
enum class state : std::uint8_t { idle = 0, running = 1, done = 2 };

state make_idle()    { return state::idle; }
bool  is_terminal(state s) { return s == state::done; }
```

Why bother? Two practical wins:

1. **Headers stay slim.** Callers don't need to see the enumerator
   list. That cuts a header dependency between modules that only ever
   pass `state` around, never branch on it.

2. **Adding enumerators doesn't recompile the world.** Add `cancelled`
   to the enum body in the `.cpp`, and only the file that switches on
   `state` recompiles. Callers who just pass it through don't.

The underlying type has to be visible at the forward declaration site
— that's how the compiler knows `sizeof(state)` (and therefore how to
pass it by value, where to align it, what register to put it in).
Default unscoped or scoped `enum`s can *not* be forward-declared
without spelling the underlying type.

## Task

This task uses **two source files** for the first time:
- `solution.hpp` — forward declaration of `state`, plus function
  *declarations*. Provided to you intact; do not edit.
- `solution.cpp` — full enum *definition* plus function bodies. This
  is what you fill in.

The functions to implement:

| Function           | Returns                            |
|--------------------|------------------------------------|
| `make_idle()`      | `state::idle`                      |
| `make_running()`   | `state::running`                   |
| `make_done()`      | `state::done`                      |
| `is_terminal(s)`   | `true` iff `s == state::done`      |
| `name_of(s)`       | `"idle"` / `"running"` / `"done"`  |

Enumerator values (define them in the `.cpp`):

| Enumerator         | Value |
|--------------------|-------|
| `state::idle`      | `0`   |
| `state::running`   | `1`   |
| `state::done`      | `2`   |

`tests.cpp` interacts with the enum entirely through the public
factory + predicate API. It can't reference `state::idle` directly
because the enumerator list isn't visible in the header.

## Run

```sh
./verify.sh
```

## Hints

- The forward declaration in `solution.hpp` already pins the
  underlying type:
  ```cpp
  enum class state : std::uint8_t;
  ```
  In `solution.cpp` you provide the full definition. The underlying
  type must match exactly — `enum class state : std::uint8_t { ... };`.

- `name_of` is most naturally a `switch (s)`. With a `default:` arm
  you handle the "future enumerator" case cleanly.

- Tests will `static_cast<std::uint8_t>(make_idle())` and friends —
  that conversion is legal even with the enum only forward-declared
  in `solution.hpp`, because the underlying type is visible there.
