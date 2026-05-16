# 12 — `enum class`

> Spec: <https://en.cppreference.com/cpp/language/enum>

## What you learn

C++11 introduced **scoped enumerations**, declared with `enum class`
(or equivalently `enum struct`). They fix three real problems with
the old C-style `enum`:

| Old `enum`                                  | `enum class`                          |
|---------------------------------------------|---------------------------------------|
| names leak into the enclosing scope         | names live inside the enum: `color::red` |
| implicitly converts to `int`                | no implicit conversion (use `static_cast`) |
| underlying type is unspecified              | defaults to `int`, can be pinned: `enum class e : std::uint8_t {}` |

The implicit-int conversion was the worst offender:

```cpp
enum color { red, green, blue };  /* C-style */
int x = red;                      /* compiles silently */
if (red == 0) { /* always true */ }

enum class scoped_color { red, green, blue };  /* C++11 */
int x = scoped_color::red;        /* error: no implicit conversion */
int x = static_cast<int>(scoped_color::red);   /* explicit: fine */
```

You still get all the comparisons and switches and so on. You just
have to spell the type when you want an int out of it.

## Task

In `solution.hpp`, declare a scoped enum and a `to_string` function:

```cpp
namespace task12 {

enum class color { red, green, blue };

const char *to_string(color c);

}
```

| Input            | Must return  |
|------------------|--------------|
| `color::red`     | `"red"`      |
| `color::green`   | `"green"`    |
| `color::blue`    | `"blue"`     |

Implement `to_string` with a `switch` on `c`. The compiler should
warn you if you forget a case (and the warning is on by default in
gcc/clang for switches over enums).

`tests.cpp` also pins compile-time properties:
- `color` is *not* implicitly convertible to `int`.
- `color::red` is in the `color::` scope (you can't reach it as bare `red`).
- `sizeof(color)` matches the default underlying type (`int`).

## Run

```sh
./verify.sh
```

## Hints

- The enum syntax is exactly as in the table above: `enum class color { red, green, blue };`.
- For `to_string`, a `switch (c)` with `case color::red:` etc. The compiler is *smart enough* to know the switch covers every enumerator if you write all three — no `default:` needed. But adding a `default: return "?";` (or similar) silences "control reaches end" warnings if a future contributor adds a new enumerator without updating the switch.
- `std::is_convertible<task12::color, int>::value` is `false` for a scoped enum. The static_assert in tests.cpp uses exactly that.
