# 46 — `noexcept` operator

> Spec: <https://en.cppreference.com/cpp/language/noexcept>

## What you learn

Step 45 was about *writing* `noexcept` on declarations. This step
is about *asking* whether something is noexcept: the **`noexcept`
operator**.

```cpp
noexcept(EXPRESSION)
```

Returns a compile-time `bool`. `true` if the expression — every
call, every conversion, every operation it contains — is
noexcept; `false` if any subexpression could throw.

The key property: **the operand is not evaluated**. Like `sizeof`
and `decltype`, `noexcept(expr)` is an *unevaluated context*. You
can write:

```cpp
noexcept(new int(0))    /* false -- allocation may throw */
noexcept(std::vector<int>{})   /* false in C++11 -- allocation */
noexcept(int{})         /* true -- value-init of int can't throw */
```

…and *no allocation happens*. The compiler inspects the expression
purely at the type / specifier level.

This is what makes `noexcept` operator the building block for
*conditional noexcept* in step 47 (`noexcept(noexcept(...))`) and
for the standard library's `is_nothrow_*` traits.

## Three things to remember

1. **Operand is unevaluated.** Side effects don't happen. Don't
   write `noexcept(launch_missiles())` expecting missiles to
   launch (or not launch).

2. **The result is a constant expression.** Usable in
   `static_assert`, as a template argument, as the second
   `noexcept(...)` operand for conditional noexcept.

3. **It looks for noexcept-ness of every subexpression.** A
   composite expression like `a() + b()` is noexcept iff *all* its
   parts are. If `a()` is noexcept but `b()` isn't, the whole
   thing isn't.

## Task

`solution.hpp` provides two declarations:

```cpp
void safe() noexcept;
void unsafe();
```

You implement four `constexpr` `bool` functions, each returning
the result of `noexcept(...)` on a specific expression:

| Function                                 | Expression queried     | Expected return |
|------------------------------------------|------------------------|-----------------|
| `calling_safe_is_noexcept()`             | `safe()`               | `true`          |
| `calling_unsafe_is_noexcept()`           | `unsafe()`             | `false`         |
| `int_default_init_is_noexcept()`         | `int{}`                | `true`          |
| `new_int_is_noexcept()`                  | `new int(0)`           | `false`         |

The body of each is one line: `return noexcept(EXPRESSION);`.

`tests.cpp` pins each return value at compile time with
`static_assert` and at runtime with `EXPECT_TRUE` / `EXPECT_FALSE`.
It also has a composite case proving the "all subexpressions must
be noexcept" rule.

## Run

```sh
./verify.sh
```

## Hints

- Each function is one line:
  ```cpp
  constexpr bool calling_safe_is_noexcept() {
      return noexcept(safe());
  }
  ```

- `new int(0)` is not noexcept because `operator new` can throw
  `std::bad_alloc`. To make a noexcept allocation, use
  `new (std::nothrow) int(0)` — the nothrow placement form.

- `int{}` is value-initialization of a built-in type. Built-in
  type init is `noexcept`. A class type with a non-noexcept default
  ctor would *not* be noexcept-init.

- The composite test in `tests.cpp` doesn't require any function
  from you — it just calls `noexcept(safe(), unsafe())` and
  asserts it's false. The comma here is the *comma operator*, not
  argument separation; both subexpressions are tested.
