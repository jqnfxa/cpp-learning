# 3 — Return-type deduction `auto f(...)`

> Spec: <https://en.cppreference.com/cpp/language/function#Return_type_deduction>

## What you learn

C++14 allows function return types to be **deduced from the
return statement**:

```cpp
auto add(int a, int b)
{
    return a + b;     /* return type deduced from this expression */
}
```

No trailing return type. No `decltype` spell. The compiler
looks at the return expressions in the body and works out the
return type the same way `auto x = expr;` works for variables.

This generalises to templates -- the deduction happens
independently for each instantiation:

```cpp
template <class A, class B>
auto add(A a, B b)
{
    return a + b;     /* deduced per (A, B) pair */
}

add(1, 2);            /* return type: int */
add(1, 2.0);          /* return type: double */
add(1, 3L);           /* return type: long */
```

### Contrast with the C++11 trailing form

In C++11, to express "the return type is whatever `a + b`
yields" you wrote:

```cpp
template <class A, class B>
auto add(A a, B b) -> decltype(a + b)
{
    return a + b;
}
```

You had to:
1. Use `auto` as a placeholder.
2. Put `decltype(a + b)` after the parameter list (so `a` and `b`
   are in scope when the type is computed).
3. Repeat the expression in the body.

That redundancy is exactly what C++14 removed. The body's
return expression is the source of truth.

### Rules of `auto`-return deduction

The deduction follows the **`auto` rules**, not the
`decltype` rules:

| Return expression | Deduced type |
|-------------------|--------------|
| `a + b` (rvalue) | `decltype(a + b)` (e.g. `int`, `double`) |
| `obj` (lvalue) | the OBJECT TYPE -- references stripped! |
| `v[0]` (returns `T&`) | `T`, not `T&` -- references stripped! |
| `const T x; return x;` | `T`, not `const T` -- top-level cv stripped |

The most surprising consequence: returning a reference-yielding
expression with bare `auto` produces a **copy**:

```cpp
template <class T>
auto first(const std::vector<T> &v)   /* returns T BY VALUE */
{
    return v[0];                       /* v[0] is const T& but auto strips */
}
```

To preserve the reference, you'd write `auto&` (declared
return reference) or `decltype(auto)` (perfect-forward the
expression's category and type). That's step 4.

### Multiple return statements

If the body has multiple `return` statements, all must yield
the **same** type:

```cpp
auto sign(int x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}    /* fine -- all three are `int` */
```

vs.

```cpp
auto bad(int x)
{
    if (x > 0) return 1;       /* int */
    return 1.0;                /* double -- compile error */
}
```

C++14 doesn't do "common type" deduction across return
statements (C++17 doesn't either). Either make them all the
same type explicitly, or use a single return with `?:`.

### Recursion needs a non-deduced return seen first

A recursive call has to know its own return type. With pure
return-type deduction, the first `return` statement in the
body fixes the type. So this is fine:

```cpp
auto fact(int n)
{
    if (n <= 1) return 1;          /* fixes the return type to int */
    return n * fact(n - 1);        /* OK -- type already known */
}
```

This is NOT fine:

```cpp
auto fact(int n)
{
    return n <= 1 ? 1 : n * fact(n - 1);     /* fact's type unknown
                                                during deduction */
}
```

The fix is either to put the base-case `return` first, or to
fall back to a trailing return type.

### Declarations need definitions to deduce

Forward-declarations of an `auto`-returning function are
allowed, but the FUNCTION DEFINITION must be visible at every
call site that needs the return type. Practically: put
auto-returning functions in headers, as templates already
have to be.

### When NOT to use return-type deduction

- **Public ABI surfaces**: changing the body could
  inadvertently change the return type, breaking callers.
- **Functions with complex bodies and multiple returns**: easy
  to introduce inconsistency.
- **Recursive functions**: see above; the deduction rules can
  make recursion awkward.

For small one-liners and templates where the return type is
"obvious from the expression," `auto` is great. For wider
APIs, the trailing-return form is more honest about the
contract.

## Task

In `solution.hpp`, fill in three function bodies. Each
declares an `auto` return type and the body should return
the obvious arithmetic combination of its two arguments:

```cpp
template <class A, class B> auto add(A a, B b);       /* a + b */
template <class A, class B> auto multiply(A a, B b);  /* a * b */
template <class A, class B> auto subtract(A a, B b);  /* a - b */
```

These are the C++14 equivalents of the C++11 `auto NAME(...)
-> decltype(...)` patterns from std11 step 61.

## Run

```sh
./verify.sh
```

## Hints

- The bodies are literally one line: `return a + b;` /
  `return a * b;` / `return a - b;`.

- The `auto` return type does the work -- you don't write any
  `decltype` or `-> ...`.

- For `add(1, 2.0)`, the body's `a + b` evaluates to
  `int + double = double`, and the deduced return type is
  `double`. The stub returns just `a` (int), so both the value
  AND the type are wrong.

- Tests verify: arithmetic values for ints/doubles/strings,
  AND `decltype(add(...)) == expected_type` via runtime
  `EXPECT_TRUE(std::is_same<...>::value)`. The type checks
  catch "I returned only `a` and lost the wider type."

- `add(string, const char*)` yields `std::string` (the
  string's `operator+` does the work). The tests include
  one such case to show the deduction is not restricted to
  arithmetic.

- These functions are NOT marked `constexpr` -- return-type
  deduction works fine in regular functions too. Step 1
  unlocked the loops-in-constexpr, step 3 unlocks bare-auto
  return types; the two are independent C++14 changes.

- For first_element-style "return a reference," use `auto&`
  (declared return reference) or `decltype(auto)` (step 4).
  Plain `auto` would strip the reference -- a common pitfall
  worth knowing.
