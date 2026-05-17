# 10 — Auto non-type template parameters

> Spec: <https://en.cppreference.com/cpp/language/template_parameters>

## What you learn

C++17 lets you write `template <auto X>` for a non-type
template parameter (NTTP) whose type is deduced from the
argument:

```cpp
template <auto N>
struct fixed_array { int data[N]; };

fixed_array<5> a;       /* N is `int`, value 5 */
fixed_array<10ULL> b;   /* N is `unsigned long long`, value 10 */
fixed_array<'A'> c;     /* N is `char`, value 65 */
```

Before C++17, the type had to be explicit:

```cpp
template <std::size_t N>
struct fixed_array { int data[N]; };

fixed_array<5> a;       /* N is size_t, value 5 (int coerced) */
```

`auto` deduces from the supplied argument, just like `auto`
in variable declarations or return types.

### How it works

`template <auto X>` declares X as a placeholder. When you
write `Template<some_constant>`, the compiler:

1. Infers X's type from `some_constant`'s type.
2. Instantiates `Template` with that type and value.

The deduction follows the same rules as `auto x = some_constant;`:

- Integer literals -> their literal type (e.g., 5 -> int,
  5ULL -> unsigned long long).
- Character literals -> char.
- Pointer-to-static -> pointer to that type.
- Reference-to-static -> reference.

### Valid types for non-type template parameters

In C++17, NTTPs can be:

- Integer types (`int`, `unsigned`, `char`, ...).
- Enum types.
- Pointers to objects or functions.
- Lvalue references to objects or functions.
- `std::nullptr_t`.
- Member pointers.

So:

```cpp
template <auto P>                    /* P could be any of the above */
struct holder { /* ... */ };

template <int N> ...                 /* explicit type */
template <auto N> ...                /* deduced */
```

C++17 doesn't allow class-type NTTPs yet (e.g.,
`template <std::string S>` is ill-formed). C++20 added
**class-type NTTPs** for structural types.

### Why this matters

For SIZES, you'd typically want `size_t`. But for FLAG
values, `int` or `unsigned` might be more natural. For
small bit-packed values, `uint8_t`. The `auto` form lets the
caller pick.

```cpp
template <auto N>
class counter { /* ... */ };

counter<5> c;         /* 5 is int -- counter uses int */
counter<5UL> c;       /* 5UL is unsigned long */
counter<5ULL> c;      /* unsigned long long */
```

Each instantiation has the matching internal type.

### Variable templates with auto

`template <auto V>` works for variable templates too:

```cpp
template <auto V>
constexpr auto identity = V;

constexpr int x = identity<42>;          /* x = 42, int */
constexpr long y = identity<42L>;        /* y = 42L, long */
```

Same deduction rule -- the type follows the argument.

### Inferring through expressions

```cpp
template <auto V>
constexpr auto twice = V * 2;

constexpr int x = twice<10>;            /* int, 20 */
constexpr double y = twice<1.5>;        /* deduces double; 1.5 is double, * 2 is double */
```

Wait -- `1.5` is `double`, but C++17 doesn't allow
floating-point NTTPs (only integer/enum/pointer). The line
above is ill-formed. C++20 added floating-point NTTPs.

For C++17:

```cpp
constexpr int x = twice<10>;            /* OK: 10 is int */
constexpr long y = twice<10L>;          /* OK: 10L is long */
constexpr auto z = twice<sizeof(int)>;  /* OK: size_t */
```

### Class template auto with explicit instantiation

You can mix `auto` with explicit type when you want:

```cpp
template <auto Min, auto Max>
struct range { /* ... */ };

range<-10, 10> r;       /* Min: int, Max: int */
range<-10, 10U> r;      /* Min: int, Max: unsigned -- different types! */
```

The two parameters' types are deduced independently. This
can be a feature or a footgun -- be careful when mixing
signed/unsigned in the same instantiation.

### Comparison: `template <T t>` vs `template <auto t>`

```cpp
template <int N>     ...      /* C++11: N is always int */
template <std::size_t N> ...  /* C++11: N is always size_t */
template <auto N>    ...      /* C++17: type deduced from argument */
```

Use `auto` when the type CAN vary (size, key, ID); use
explicit when the type is fixed (always an int, always a
size_t).

### Limitations

- C++17 NTTPs can't be class types. `template <auto S =
  some_struct{}>` doesn't work in C++17. C++20 added it via
  structural types.
- The argument MUST be a constant expression.
- For pointers/references, the target must have static
  storage duration.

## Task

In `solution.hpp`, change two template-parameter
declarations from explicit types to `auto`:

```cpp
template <auto N>
struct fixed_array { int data[N]; ... };

template <auto V>
struct value_holder { ... };
```

The internal members and methods stay the same -- they
already use `N` / `V` in ways that don't depend on the
specific underlying type.

## Run

```sh
./verify.sh
```

## Hints

- Change `template <std::size_t N>` to `template <auto N>`
  for `fixed_array`.

- Change `template <int V>` to `template <auto V>` for
  `value_holder`.

- The internal `data[N]` array still works -- `N` is now
  whatever type the user supplied (int, ULL, char, ...) at
  the instantiation site.

- `size()` returns `N` directly; with `template <auto N>`,
  the return type follows `N`'s deduced type. The stub's
  `template <size_t N>` forces the return to size_t
  regardless.

- For `value_holder`, `value()` returns `V` and follows the
  same pattern.

- Tests pin the deduced type with `is_same<decltype(...),
  expected>`. The KEY tests verify that `fixed_array<10ULL>`
  gives `unsigned long long` size, not size_t.

- C++17 NTTPs: integer/enum/pointer/reference/nullptr_t/
  member-pointer. NOT: floating-point, class types, lambdas.
  C++20 lifted some of these limits.

- For unmatched-type parameters, the deduction picks ONE
  type per parameter (independently). `range<-10, 10U>` has
  Min=int and Max=unsigned -- mixed signs in same
  template! Be careful.

- The `auto` placeholder doesn't help you GUARD against
  wrong-type arguments -- if you need exactly `size_t`,
  write `template <std::size_t N>` explicitly.

- Modern STL containers don't generally use `auto` NTTPs
  (they pin specific types like `size_t`). The `auto` form
  shines in user code where the call-site's literal should
  drive the storage type.
