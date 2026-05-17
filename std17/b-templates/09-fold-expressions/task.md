# 9 — Fold expressions

> Spec: <https://en.cppreference.com/cpp/language/fold>

## What you learn

C++17 added **fold expressions**: a concise syntax to reduce
a parameter pack with an operator. Four flavors:

| Form           | Syntax                  | Expansion                |
|----------------|-------------------------|--------------------------|
| Unary left     | `(... op pack)`         | `((p1 op p2) op p3) op ...` |
| Unary right    | `(pack op ...)`         | `p1 op (p2 op (p3 op ...))` |
| Binary left    | `(init op ... op pack)` | `((init op p1) op p2) op ...` |
| Binary right   | `(pack op ... op init)` | `p1 op (p2 op (... op init))` |

The `...` is the literal token in both fold syntax and pack
expansion. The compiler unfolds it at compile time.

### The C++17 replacement for the dummy-array trick

C++14 needed `int dummy[]{0, (expr, 0)...}` for "do
something for each element of a pack." C++17 reduces that
to a fold:

```cpp
/* C++14 */
int result = 0;
int dummy[] = {0, (result += args, 0)...};
(void)dummy;

/* C++17 */
int result = (0 + ... + args);   /* binary left fold */
/* or */
int result = (... + args);       /* unary left fold; requires non-empty pack */
```

Same effect, one line instead of three.

### The 32 operators

Fold works with these operators:

```
+   -   *   /   %   ^   &   |   <<   >>
+=  -=  *=  /=  %=  ^=  &=  |=  <<=  >>=
=   ==  !=  <   >   <=  >=  &&  ||   ,
.*  ->*
```

So you can fold across arithmetic, bitwise, logical,
assignment, comparison, comma, and member-access operators.
The most useful in practice are `+`, `*`, `&&`, `||`, `,`,
and `==`.

### Unary vs binary

```cpp
(... + args)         /* unary left -- ill-formed for empty pack */
(0 + ... + args)     /* binary left -- works for empty pack (yields 0) */
```

Unary fold over an empty pack is **ill-formed for most
operators**. The exceptions (where the standard defines an
identity for the empty case):

- `&&` -> `true`
- `||` -> `false`
- `,` -> `void()` (no-op)

For other operators, use the binary form with an explicit
identity:

```cpp
(0 + ... + args)     /* sum -- identity 0 */
(1 * ... * args)     /* product -- identity 1 */
(true && ... && args)  /* all_true -- identity true */
```

### Left vs right associativity

```cpp
(... - args)       /* ((p1 - p2) - p3) - ... -- left associative */
(args - ...)       /* p1 - (p2 - (p3 - ...)) -- right associative */
```

For commutative operators (`+`, `*`, `&&`, `||`), the
direction doesn't matter. For non-commutative operators
(`-`, `/`, `<<`), pick the direction that matches your
intent. For most use cases (sum, product, all/any), left
fold is fine.

### The comma operator fold

The most common "do something for each" pattern:

```cpp
template <class... Args>
void print_all(Args const&... args)
{
    ((std::cout << args << "\n"), ...);   /* unary fold over , */
}
```

Each statement runs in order. The fold yields `void()`
because each `<<` is a side-effecting expression.

This replaces the C++14 `int dummy[]{0, ((std::cout << args), 0)...};`.

### Working with parameter packs

The pack on the right (or left) of `...` is any
non-empty-after-deduction parameter pack:

```cpp
template <class... Args>
auto sum(Args... args) {
    return (... + args);      /* args is the pack */
}

template <class T, class... Rest>
auto all_in(T head, Rest... tail) {
    return (head == ... == tail);  /* binary fold -- this is a *weird* expression
                                       and you should NOT actually write this;
                                       it's here to show the syntax */
}
```

In real code, fold expressions are simple: sum, product,
all/any, do-for-each. Complex compound expressions are
rare.

### When NOT to use fold

- **For ONE argument**: just write it inline.
- **For variable counts at runtime**: fold is compile-time;
  use a container + std::accumulate / std::reduce.
- **For complex per-element processing**: the comma fold is
  legal but quickly becomes hard to read. For non-trivial
  loops, use a helper function template.

### Examples in the standard library

C++17's `<numeric>` got `std::reduce` -- the parallel
counterpart to `accumulate`. The standard library uses fold
expressions internally for variadic templates (e.g.,
`std::apply`'s helper).

`std::invoke` (C++17) and `std::apply` use a combination of
fold + index_sequence for argument forwarding.

### Compile-time evaluation

Fold expressions are evaluated at compile time IF the
arguments are constexpr. Since most pack elements are
template parameters (compile-time types), the fold itself is
typically a runtime expression on values; but the
*expansion* happens at compile time.

```cpp
constexpr int s = sum(1, 2, 3);   /* OK -- constexpr expansion */
```

## Task

In `solution.hpp`, fill in four templates:

```cpp
template <class... Args>
auto sum(Args... args) { return (... + args); }

template <class... Args>
auto product(Args... args) { return (... * args); }

template <class... Args>
bool all_true(Args... args) { return (... && args); }

template <class... Args>
std::string concat_with_spaces(Args const&... args) {
    std::ostringstream oss;
    ((oss << args << " "), ...);
    return oss.str();
}
```

## Run

```sh
./verify.sh
```

## Hints

- `(... + args)` is a unary left fold over `+`. Same as
  `((((p1 + p2) + p3) + p4) + ...)`. The pack
  `args` must be non-empty.

- `(... * args)` is the same for `*`.

- `(... && args)` is the same for `&&`. Useful for
  "all-of"-style queries on packs.

- `((oss << args << " "), ...)` is a unary right fold over
  the comma operator. Each `oss << arg << " "` runs in
  order; the result of each is discarded (comma operator).
  The parentheses around `oss << args << " "` are required
  -- the precedence of `,` would otherwise make the
  expression parse differently.

- For the `concat_with_spaces` test "concat_one_int" --
  with one arg, the fold runs once: `oss << 42 << " "`.
  Result: `"42 "` (a trailing space). Tests pin this.

- `oss.str()` extracts the string from the stream.

- `<sstream>` is included via solution.hpp.

- `(void)sizeof...(args)` in the stub silences the "unused
  parameter pack" warning. Don't leave that in the
  reference.

- Tests verify: sum across 2/3/5 ints and mixed int+double;
  product 2/3 ints and one-with-zero; all_true with all
  true, one false, and predicate args; concat for 1/2/3
  arguments of mixed types.

- For C++14 backward port, you'd write the dummy-array
  trick. For C++17+, always prefer fold expressions --
  shorter, clearer, faster to compile.

- C++17 also added `std::reduce` (in `<numeric>`) as a
  parallel-able alternative to `std::accumulate`. For
  compile-time pack reduction, use fold; for runtime range
  reduction, use std::reduce.
