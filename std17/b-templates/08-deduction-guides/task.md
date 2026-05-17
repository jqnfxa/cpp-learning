# 8 — User-defined deduction guides

> Spec: <https://en.cppreference.com/cpp/language/class_template_argument_deduction>

## What you learn

Step 7 introduced CTAD. The compiler synthesizes deduction
guides from each constructor automatically. Sometimes that's
not what you want:

```cpp
template <class T>
struct wrapper { T value; wrapper(T v); };

wrapper w("hello");    /* implicit guide: wrapper<const char*> */
```

The implicit guide deduces `T = const char*`, not
`std::string`. If you want the wrapper to hold a
`std::string` when given a literal, you write a **user-
defined deduction guide**:

```cpp
wrapper(const char*) -> wrapper<std::string>;
```

Now:

```cpp
wrapper w("hello");    /* user-defined guide: wrapper<std::string> */
```

The guide says: "when CTAD sees `wrapper(const char*)`,
deduce `wrapper<std::string>` instead of letting the
implicit guide pick `wrapper<const char*>`."

### Syntax

```cpp
template <template-parameters>           /* optional */
ClassName(arg-types) -> ClassName<deduced-types>;
```

- Lives at **namespace scope**, OUTSIDE the class body.
- The `->` is part of the syntax (NOT a function pointer).
- The right side names the class with specific template args.
- The template parameter list is optional; if omitted, the
  guide deduces from the call site's args directly.

Examples:

```cpp
/* Map (const char*) to wrapper<std::string> */
wrapper(const char*) -> wrapper<std::string>;

/* Generic: deduce T from any arg, but always use the decayed form */
template <class T>
wrapper(T) -> wrapper<std::decay_t<T>>;

/* Two-arg constructor; deduce both T1 and T2 from args */
template <class T1, class T2>
pair(T1, T2) -> pair<T1, T2>;     /* (this is essentially the implicit guide) */
```

### Overload resolution

User-defined guides participate in **overload resolution**
alongside the implicit guides. The compiler picks the best
match by normal overload rules.

For `wrapper(const char*) -> wrapper<std::string>;`:
- The user-defined guide takes `const char*` directly.
- The implicit guide is `wrapper(T) -> wrapper<T>`.
- For `wrapper w("hello")`, the user-defined guide is a
  better match (exact `const char*`); it wins.

If you had two user-defined guides for the same input, you'd
get an ambiguity error.

### Common patterns

**Container from a range**:

```cpp
template <class It>
struct range { range(It b, It e); };

template <class Container>
range(Container&) -> range<typename Container::iterator>;
```

This guide lets `range r(some_vector)` deduce
`range<vector<int>::iterator>` from the container's iterator
type, without needing to specify it.

**Forcing the decay**:

```cpp
template <class T>
struct holder { T value; };

template <class T>
holder(T) -> holder<std::decay_t<T>>;
```

By default, CTAD might deduce reference or cv-qualified
types. The user-defined guide forces the `decay_t` form for
storage-safe values.

**Mapping types**:

```cpp
template <class T>
struct buffer { /* ... */ };

template <class T>
buffer(T*, std::size_t) -> buffer<std::remove_const_t<T>>;
```

A guide can strip qualifiers, transform types, etc.

### The std::vector trap

`std::vector` has user-defined deduction guides in the
standard library:

```cpp
std::vector v{1, 2, 3};      /* deduces vector<int> */

std::vector v(it1, it2);     /* deduces vector<value_type_of_iterator> */
```

Without a user-defined guide for the iterator-pair form, the
ambiguity between "an initializer list of two iterators" and
"an iterator pair" would be problematic. The standard
library provides the guide that says "two-iterator form is
the iterator-pair constructor."

### When to write your own

- Your class template has a constructor that takes a generic
  type but you want to store something different (e.g., a
  literal -> a string).
- Your template has multiple constructors and the implicit
  guides ambiguate.
- You want CTAD to work even though the implicit guides
  don't cover your case (e.g., extra template parameters
  you can derive from the args).

For most simple class templates, the implicit guides are
sufficient; reach for user-defined guides only when CTAD
doesn't do what you want.

### A note on aggregates

In C++17, aggregates (classes without user-declared
constructors) **don't get implicit deduction guides**.
CTAD on a raw aggregate fails:

```cpp
template <class T>
struct pair_agg { T a; T b; };

pair_agg p{1, 2};       /* C++17 error: no CTAD for aggregates */
```

You'd need an explicit guide:

```cpp
template <class T>
pair_agg(T, T) -> pair_agg<T>;

pair_agg p{1, 2};       /* C++17: OK now */
```

C++20 extended CTAD to aggregates, but for std17/, you write
the guide explicitly.

### Limitations

- Guides can't be template friends.
- Guides can't have default arguments.
- Guides are looked up via normal name lookup (in the
  namespace of the template).
- A guide's parameters must satisfy the template's
  constructor's parameter signature for it to be "best
  match" -- it's not a magical override, just an overload
  participant.

## Task

In `solution.hpp`, add a user-defined deduction guide and
fill in a helper function:

```cpp
/* The guide */
wrapper(const char*) -> wrapper<std::string>;

/* The helper */
inline std::string wrap_literal() {
    wrapper w("hello");
    return w.value;
}
```

The guide lives at namespace scope inside `task08`, after
the `wrapper` template definition.

## Run

```sh
./verify.sh
```

## Hints

- The deduction guide goes between `wrapper` and the helper
  function in `solution.hpp`, at namespace scope.

- Syntax:
  ```cpp
  wrapper(const char *) -> wrapper<std::string>;
  ```
  No `template <...>` here -- the LHS has no template
  parameters (just `const char *` as a fixed type).

- `wrap_literal`'s body uses CTAD on a literal:
  ```cpp
  wrapper w("hello");
  return w.value;
  ```
  With the guide, `w` has type `wrapper<std::string>` and
  `w.value` is a `std::string`. The return is implicit
  conversion (`std::string` -> `std::string`).

- Without the guide, `wrapper w("hello")` deduces
  `wrapper<const char *>`, and `w.value` is `const char *`.
  The test `direct_literal_deduces_string` catches this.

- Other CTAD instantiations (int, double, std::string) keep
  working via the implicit guide -- the user-defined guide
  only adds a new mapping for `const char *`.

- The `wrap_literal_type_is_string` test verifies that the
  RETURN type is `std::string` -- which it is in both
  stub and reference (the function signature returns
  `std::string`). The TEST is here to confirm the return
  signature, not the internal deduction.

- For C++20 aggregate CTAD, raw aggregates also get implicit
  guides. C++17 only does CTAD on user-declared
  constructors.

- The standard library uses user-defined guides extensively
  (`vector`, `pair`, `tuple`, `array`, `function`,
  `shared_ptr`, etc.). Read those headers to see real-world
  guide patterns.
