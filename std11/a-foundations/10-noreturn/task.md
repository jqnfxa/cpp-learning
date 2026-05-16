# 10 — Attributes: `[[noreturn]]`

> Spec: <https://en.cppreference.com/cpp/language/attributes/noreturn>

## What you learn

C++11 introduced a standard attribute syntax, `[[name]]`. Before
C++11 every compiler had its own spelling — `__attribute__((noreturn))`
on GCC, `__declspec(noreturn)` on MSVC. C++11 normalized it.

**`[[noreturn]]`** declares that a function *never returns to its
caller*. The function exits the program some other way: throws, calls
`std::exit` / `std::abort`, terminates the thread, or loops forever.

Three things the compiler does with this:

1. **Silences "missing return" warnings at call sites.** A function
   that ends with a call to a `[[noreturn]]` function doesn't need a
   trailing return statement — the compiler knows control never reaches
   the end.

2. **Optimizes the call.** No need to preserve caller-save registers
   across the call, because we're not coming back.

3. **Diagnoses violations.** If a `[[noreturn]]` function does in fact
   reach its closing `}`, that's undefined behavior — and the compiler
   typically warns about it.

```cpp
[[noreturn]] void panic(const std::string &msg)
{
    throw std::runtime_error(msg);
}

int positive_only(int n)
{
    if (n >= 0) return n;
    panic("expected non-negative input");
    /* no return statement needed -- panic is [[noreturn]] */
}
```

Without `[[noreturn]]` on `panic`, the compiler would warn
"control reaches end of non-void function" inside `positive_only`.

## Task

In `solution.hpp` there is a `panic(msg)` function declared but not
attributed and not throwing, plus a `positive_only(n)` that *relies*
on `panic` being `[[noreturn]]`. You need to:

1. Add `[[noreturn]]` to `panic`.
2. Make `panic` throw `std::runtime_error(msg)`.

| Call                          | Expected                       |
|-------------------------------|--------------------------------|
| `positive_only(5)`            | returns `5`                    |
| `positive_only(0)`            | returns `0`                    |
| `positive_only(-1)`           | throws `std::runtime_error`    |
| `panic("hello")`              | throws `std::runtime_error("hello")` |

The CMakeLists for this task adds `-Werror=return-type`. If you
forget the `[[noreturn]]`, the build itself fails on `positive_only`
("control reaches end of non-void function"). This makes the
attribute load-bearing, not just decorative.

## Run

```sh
./verify.sh
```

## Hints

- `[[noreturn]]` goes immediately before the return type (most
  common placement):
  ```cpp
  [[noreturn]] void panic(const std::string &msg);
  ```
  It can also follow the parameter list, but the leading position
  is conventional.
- The attribute itself doesn't *cause* the function to not return —
  it only *promises* that the body won't. The throw is what makes
  the promise true. If you add `[[noreturn]]` but the body falls
  through, that's UB and most compilers warn about it.
- `std::runtime_error` is in `<stdexcept>`. Its constructor takes a
  `const std::string &` or `const char *`.
