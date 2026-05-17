# 4 — `decltype(auto)`

> Spec: <https://en.cppreference.com/cpp/language/auto#decltype.28auto.29>

## What you learn

Bare `auto` for return-type deduction (step 3) follows
**`auto` rules**: references stripped, top-level cv stripped.
That's wrong for "perfect-forwarding" wrappers and for any
function that intentionally returns a reference.

C++14's `decltype(auto)` is a separate return-type placeholder
that follows **`decltype` rules** instead. The return type is
exactly `decltype(return_expression)`:

```cpp
std::vector<int> v{1, 2, 3};

auto           f1(std::vector<int> &v) { return v[0]; }   /* returns int  */
decltype(auto) f2(std::vector<int> &v) { return v[0]; }   /* returns int& */
```

Same body. Same expression. Different return types because
the deduction rules differ.

### `auto` vs `decltype(auto)` -- the three cases

The two placeholders diverge in three predictable ways:

| Return expression       | `auto` deduces | `decltype(auto)` deduces |
|-------------------------|----------------|--------------------------|
| `v[0]` (returns `T&`)   | `T`            | `T&`                     |
| `*p` (returns `T&`)     | `T`            | `T&`                     |
| `std::move(x)` (`T&&`)  | `T`            | `T&&`                    |
| `const T x; return x;`  | `T`            | `T` (declared type)      |
| `const T x; return (x);`| `T`            | `const T&` (parens!)     |

The last two rows show the **parenthesis trap**:
`decltype(NAME)` returns the declared type of `NAME`, but
`decltype((NAME))` (with parens) is the expression form and
returns the lvalue reference if NAME is an lvalue.

```cpp
decltype(auto) bad()
{
    int x = 0;
    return (x);     /* returns int& -- to a local. UB! */
}

decltype(auto) good()
{
    int x = 0;
    return x;       /* returns int -- the declared type */
}
```

The parens issue is *the* footgun of `decltype(auto)`.
Whenever you see `return (something);` in a `decltype(auto)`
function, double-check that the something is not a local.

### The motivating use case: perfect forwarding

The classic case is a thin wrapper that should preserve the
exact return-type-and-category of an inner call:

```cpp
template <class F, class... Args>
decltype(auto) invoke(F &&f, Args &&... args)
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}
```

If `f()` returns a reference, `invoke` returns a reference.
If `f()` returns by value, `invoke` returns by value. If `f()`
returns an rvalue reference, `invoke` returns an rvalue
reference. Bare `auto` couldn't do that -- it always strips.

The same logic applies to container accessors, proxy objects,
property getters, anything where the wrapper should be
**transparent** to the underlying expression's category.

### When to choose which

Default to `auto` unless you have a specific reason to
preserve a reference. Plain `auto` is safer (no UB-via-parens
trap) and clearer at the call site (the caller always gets a
value).

Choose `decltype(auto)` when:

- You're writing a **forwarding** wrapper.
- You want to **return a reference into something the caller
  owns** -- e.g., `T &back() { return data_[size_-1]; }` style.
- You explicitly want to preserve the cv-ref qualifiers of the
  underlying expression.

Don't reach for it for "general-purpose" deduction. The
parens trap WILL bite.

### `decltype(auto)` on variables

`decltype(auto)` also works as a **variable declaration**:

```cpp
int x = 5;
int &r = x;
decltype(auto) y = r;     /* y is int& */
decltype(auto) z = 5;     /* z is int */
```

Same rules as for the return type. Most uses are in template
metaprogramming where you want to forward an expression's
category.

### The interaction with `auto&` and `const auto&`

If you only need to preserve "is this a reference?" but not
"is it const or rvalue?", `auto&` or `const auto&` may
suffice:

```cpp
auto&        f(vec<int> &v) { return v[0]; }   /* int&        */
const auto&  f(vec<int> &v) { return v[0]; }   /* const int&  */
decltype(auto) f(vec<int> &v) { return v[0]; }   /* int&      */
```

`auto&` is a bit more obvious to readers than `decltype(auto)`
and doesn't have the parens trap. Use `decltype(auto)` when
the inner expression could be ANY of {T, T&, T&&, const T&,
...} and you want to preserve whichever it is.

## Task

In `solution.hpp`, change the return type of `decltype_first`
from bare `auto` to `decltype(auto)`. The body
(`return v[0];`) stays the same. The function's BEHAVIOR
changes from "return a copy" to "return a reference".

`auto_first` stays `auto` -- it's the side-by-side
demonstration.

```cpp
template <class T>
decltype(auto) decltype_first(std::vector<T> &v)
{
    return v[0];
}
```

## Run

```sh
./verify.sh
```

## Hints

- One change to make: `auto` -> `decltype(auto)` on
  `decltype_first`'s return type. Body unchanged.

- After the change, `decltype(decltype_first(v))` is `T&`,
  not `T`. Tests verify this with
  `EXPECT_TRUE(std::is_same<R, int &>::value)`.

- Address test: `const auto &r = decltype_first(v); EXPECT_EQ(&r, &v[0]);`.
  - If `decltype_first` returns `int&`, `r` binds to `v[0]`
    directly. `&r == &v[0]`.
  - If `decltype_first` returns `int` (the stub), `r`
    lifetime-extends a temporary. `&r != &v[0]`.

- Mutation test: with the real reference, mutating `v[0]`
  shows up through `r` (it's the SAME storage). With the
  stub, `r` is an independent copy.

- The auto-first tests are control cases: they always pass
  because `auto_first` always strips the reference.

- The tests do NOT use `static_assert(std::is_same<R, int&>...)`
  -- that would prevent the stub from compiling. Runtime
  `EXPECT_TRUE` keeps the stub-vs-reference workflow alive.

- Tests cover: value sanity, int and string element types,
  type checks for both functions, address aliasing in both
  positive and negative directions, mutation-through-reference
  proof.

- The parens trap: `decltype(auto) f() { int x; return (x); }`
  returns `int&` to a local -- UB. Avoid `return (expr)` in
  any `decltype(auto)` function unless you're certain `expr`
  is not a local.
