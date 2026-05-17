# 45 — `noexcept` specifier

> Spec: <https://en.cppreference.com/cpp/language/noexcept_spec>

## What you learn

`noexcept` on a function declaration is a **compile-time
contract**: "this function does not throw." The compiler:

1. Lets type-trait queries like `is_nothrow_invocable` / the
   `noexcept(...)` operator see the noexcept-ness of the
   declaration.
2. Generates *no* exception-unwinding machinery at call sites
   inside the function (small codegen benefit).
3. If a `noexcept` function does throw, **`std::terminate` is
   called**. The exception doesn't propagate; the program ends.

Step 35 already touched the *move-ops + container* angle of
noexcept. This step is broader: any function — free function,
member, lambda — can be marked.

### Two forms

```cpp
void f() noexcept;          /* "this function does not throw" */
void g() noexcept(false);   /* "this function MIGHT throw" -- same as omitting */
void h() noexcept(EXPR);    /* conditional: noexcept iff EXPR is true (step 47) */
```

The trailing `noexcept` goes after the function signature, before
any function body or trailing return type. On member functions, it
goes *after* `const` and any `&` / `&&` ref-qualifier.

### Inference and defaults

- A function without `noexcept` is treated as `noexcept(false)`
  (might throw).
- A **destructor** defaults to `noexcept(true)` in C++11.
- A move constructor / move assignment defaulted with `= default`
  is `noexcept` iff every subobject's corresponding move op is
  `noexcept`. (Step 35 leans on this.)

## Task

In `solution.hpp`, declare and implement functions and a class
with specific noexcept profiles. `tests.cpp` verifies each profile
via the `noexcept(...)` operator (step 46 is about that operator
itself; here you just have to mark the declarations correctly).

```cpp
namespace task45 {

class widget {
public:
    int clone() const noexcept;     /* must be noexcept */
    int compute() const;             /* must NOT be noexcept (default) */
};

int free_safe() noexcept;            /* must be noexcept */
int free_unsafe();                    /* must NOT be noexcept */

}
```

Each body is `return 0;` (or similar). The *return values* don't
matter for this task; the *specifier* on each declaration is what
the tests look at.

| Expression                          | `noexcept(...)` |
|-------------------------------------|------------------|
| `widget().clone()`                  | `true`           |
| `widget().compute()`                | `false`          |
| `free_safe()`                       | `true`           |
| `free_unsafe()`                     | `false`          |

## Run

```sh
./verify.sh
```

## Hints

- For a member function, `noexcept` goes *after* `const`:
  ```cpp
  int clone() const noexcept { return 0; }
  ```
  Not `noexcept const`. Not before the function name. After the
  parameter list and any qualifiers, before any trailing return
  type or function body.

- The bodies can do anything (or nothing). The compiler doesn't
  inspect what's inside to decide noexcept-ness — it only reads
  the specifier. (That's why you can lie: a function marked
  `noexcept` that throws calls `std::terminate` at runtime; the
  compiler doesn't second-guess you at compile time.)

- The default — no `noexcept` at all — is *can throw*. To make a
  function "definitely throws", you'd write `noexcept(false)`,
  but it's the same as omitting.

- A common mistake: `int f() throw()` (the C++98 dynamic
  exception spec) is *deprecated and removed* in C++17. Don't
  use it. `noexcept` is the modern replacement.
