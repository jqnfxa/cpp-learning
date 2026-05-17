# 7 — Class template argument deduction (CTAD)

> Spec: <https://en.cppreference.com/cpp/language/class_template_argument_deduction>

## What you learn

Before C++17, instantiating a class template required
spelling out every template argument:

```cpp
std::pair<int, double> p(1, 2.5);
std::vector<int> v{1, 2, 3};
std::tuple<int, std::string> t(1, "hello");
```

The workaround was a factory function:

```cpp
auto p = std::make_pair(1, 2.5);    /* deduces from args */
auto v = std::vector<int>{1, 2, 3}; /* still needs <int> */
```

`std::make_pair` was specifically written to deduce from
its arguments. `std::vector` couldn't be a function (it's a
container), so you wrote the type explicitly.

C++17 added **Class Template Argument Deduction** (CTAD):
the compiler deduces the template arguments of a class
template from the constructor call:

```cpp
std::pair p(1, 2.5);              /* deduces pair<int, double> */
std::vector v{1, 2, 3};           /* deduces vector<int> */
std::tuple t(1, "hello"s);        /* deduces tuple<int, string> */
```

No more explicit `<...>` for the common case.

### How it works

When you write `std::pair p(1, 2.5)`, the compiler:

1. Looks at the constructor argument types (`int` and
   `double`).
2. Matches them against `std::pair`'s constructors.
3. Synthesizes the template-parameter values that make the
   constructor match: `T1 = int, T2 = double`.
4. Instantiates `std::pair<int, double>`.

The mechanism uses **implicit deduction guides** generated
from each constructor:

```cpp
template <class T1, class T2>
struct pair {
    pair(T1 a, T2 b);
};

/* Compiler-synthesized guide */
template <class T1, class T2>
pair(T1, T2) -> pair<T1, T2>;
```

Each constructor produces one guide. The deduction guide is
how the compiler maps "I see (1, 2.5)" to "instantiate
pair<int, double>".

### Where CTAD works

```cpp
std::pair p(1, 2.5);                /* OK */
std::pair p{1, 2.5};                /* OK */
std::vector v{1, 2, 3};             /* OK -- initializer-list */

new std::pair(1, 2.5);              /* OK -- since C++20 */
```

The `new` form needed C++20; in C++17 it required explicit
arguments. The most common patterns (declared variable +
function-style or brace init) work in C++17.

### When CTAD ambiguates or fails

CTAD has subtle rules. Some common gotchas:

- **Multiple matching constructors**: if `T(args)` matches
  more than one constructor with different deduction
  guides, deduction fails (ambiguity).

- **Implicit conversion needed**: if no constructor matches
  exactly, CTAD doesn't try implicit conversions.

- **Non-deducible parameter**: some template parameters
  can't be deduced from the constructor (e.g., the
  comparator on `std::set`). For those, you still need
  explicit args.

When CTAD fails, the error is "no matching constructor" --
you'd add explicit template args.

### User-defined deduction guides (step 8)

For your own templates, the compiler synthesizes guides from
each constructor. But sometimes you want a guide that maps
input differently (e.g., deduce a span's size from a
literal). You can declare your own:

```cpp
template <class T>
struct span {
    span(T* p, std::size_t n);
};

/* User-defined guide */
template <class T, std::size_t N>
span(T (&arr)[N]) -> span<T>;   /* C array deduces to span<T> */
```

Step 8 covers user-defined guides.

### CTAD vs `auto`

```cpp
auto p1 = std::pair{1, 2.5};        /* auto + CTAD */
std::pair p2{1, 2.5};               /* CTAD only */
auto p3 = std::make_pair(1, 2.5);   /* auto + factory */
std::pair<int, double> p4(1, 2.5);  /* no deduction */
```

All four produce the same `pair<int, double>`. The CTAD
forms are shorter; the factory form is the C++11 pattern.

### Default arguments

Class templates with default template arguments deduce them
when not supplied:

```cpp
template <class T = int>
struct holder { T value; };

holder h{};        /* CTAD: holder<int> (the default) */
holder h{1.0};     /* CTAD: holder<double> */
```

The default kicks in when the constructor has no arguments
to deduce from.

### Inheritance

CTAD works through public bases:

```cpp
template <class T>
struct base { base(T t); };

template <class T>
struct derived : base<T> {
    using base<T>::base;        /* inherit ctors */
};

derived d{42};   /* CTAD: derived<int> */
```

The inheriting `using base<T>::base;` makes `base`'s
constructor visible; the deduction guide is generated
through it.

### When NOT to use CTAD

- **Public APIs / type-system clarity**: if the deduced type
  isn't obvious to readers, write it out. `std::pair p(1,
  2.5)` is clear; `std::variant v{1, 2}` is less so (does
  it deduce variant<int> or variant<int, int>?).

- **Templates that take a separate `Allocator` parameter**:
  these often have non-deducible defaults. Be explicit when
  it matters.

- **Initializer lists**: `std::vector v{1};` deduces
  `vector<int>` with one element. `std::vector v{1, 2};`
  deduces `vector<int>` with two elements. But
  `std::vector<int> v{1};` is unambiguous. Don't rely on
  CTAD when the source has special initializer-list
  semantics.

## Task

In `solution.hpp`, fill in three factory functions to use
CTAD:

```cpp
inline std::pair<int, double> make_pair_via_ctad(int a, double b) {
    std::pair p(a, b);
    return p;
}

inline std::vector<int> make_vector_via_ctad(int a, int b, int c) {
    std::vector v{a, b, c};
    return v;
}

inline std::tuple<int, std::string, double>
make_tuple_via_ctad(int x, std::string name, double score) {
    std::tuple t{x, std::move(name), score};
    return t;
}
```

Each function declares a local with CTAD (no explicit
`<...>`), then returns it. The return type is spelled
out for clarity, but the local construction relies on
deduction.

## Run

```sh
./verify.sh
```

## Hints

- `std::pair p(a, b)` deduces `pair<int, double>` from the
  arguments' types.

- `std::vector v{a, b, c}` deduces `vector<int>` from the
  initializer-list elements (all int).

- `std::tuple t{x, std::move(name), score}` deduces from
  three argument types.

- `std::move(name)` on the string moves it into the tuple
  rather than copying. The tuple's elements are constructed
  by perfect-forwarded args.

- The return types are explicit (`std::pair<int, double>`,
  `std::vector<int>`, etc.) -- this is good practice for
  return types on a function signature, even when CTAD
  works inside.

- The "direct deduction" tests (`direct_pair_deduction`,
  etc.) exercise CTAD inline -- you write `std::pair p{...}`
  and verify `decltype(p) == std::pair<int, double>`.

- For "pair_from_strings": `std::pair p{std::string("a"),
  std::string("b")}` deduces `pair<string, string>`.

- Tests cover: pair values (signs of int and double),
  vector contents and ordering, tuple field access, and
  type-check (is_same on the deduced type for each
  direct-deduction test).

- For step 8 (user-defined deduction guides), the same
  mechanism is exposed for user templates -- the compiler
  uses your guides instead of (or in addition to)
  auto-generated ones.

- For older code bases: `std::make_*` factories still
  work. CTAD is a syntactic improvement, not a behavioral
  change.
