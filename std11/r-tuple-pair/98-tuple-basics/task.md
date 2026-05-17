# 98 — `std::tuple<...>` basics

> Spec: <https://en.cppreference.com/cpp/utility/tuple>

## What you learn

`std::tuple<T1, T2, ..., Tn>` is the generalisation of
`std::pair<T1, T2>` to N elements. It's a fixed-size,
heterogeneous record — each slot can have a different type:

```cpp
std::tuple<int, std::string, double> record(1, "alice", 3.14);
```

The "fixed-size, heterogeneous" combination is what makes tuples
useful where you'd otherwise write a one-off `struct` and never
use it again:

```cpp
/* Used to be: */
struct Result { int id; std::string name; double score; };
Result get_record();

/* Now sometimes: */
std::tuple<int, std::string, double> get_record();
```

When the components have natural names that you'll reference
many times, prefer the struct. When the tuple is a temporary
return value the caller will immediately destructure, the
tuple is shorter.

### Construction

```cpp
std::tuple<int, std::string, double> t1(1, "x", 3.14);   /* direct */
auto t2 = std::make_tuple(1, std::string("x"), 3.14);    /* deduction */
std::tuple<int, std::string, double> t3{1, "x", 3.14};   /* brace-init */
```

`std::make_tuple` deduces the element types from its arguments,
which is sometimes shorter (no need to write the full type) and
sometimes risky (deduces `const char *` instead of
`std::string`, etc.). For "return this tuple from a function
whose signature already declares it", direct construction or
brace-init is usually cleaner. Step 100 covers `make_tuple` and
`forward_as_tuple` in depth.

### Element access via `std::get<I>`

```cpp
int         id    = std::get<0>(t);
std::string name  = std::get<1>(t);
double      score = std::get<2>(t);
```

`std::get<I>(t)` is the access primitive — index by position at
compile time. Wrong index is a compile error, not a runtime
failure:

```cpp
std::get<3>(t);   /* compile error: index out of range */
```

C++14 added `std::get<T>(t)` (index by type, when unique) for
cases where positional indexing is too brittle to read. C++17
added structured bindings (`auto [id, name, score] = t;`) which
makes the tuple-to-locals dance two lines shorter. For C++11
you use the positional form.

### Modification and assignment

`std::get<I>(t)` returns a reference, so you can write into a
tuple element:

```cpp
std::get<1>(t) = "bob";
```

And whole-tuple assignment works element-wise:

```cpp
t = std::make_tuple(2, std::string("bob"), 2.71);
```

### Why use a tuple?

Three main reasons:

1. **Multiple return values without a struct.** "I want to
   return an index *and* a value" is naturally a 2-tuple.
2. **Generic, variadic code.** Templates that take "any number
   of arguments of any types" model their state as a tuple.
3. **Lexicographic comparison.** Tuples compare element-wise
   left-to-right, which is exactly what you want for composite
   sort keys (sort by department first, then by salary, then
   by name -> sort by `std::tie(dept, salary, name)`).

## Task

In `solution.hpp`, implement one function:

```cpp
namespace task98
{
    using record = std::tuple<int, std::string, double>;

    /* Construct and return a record with the given fields. */
    record make_record(int id, std::string name, double score);
}
```

Tests use `std::get<0>`, `std::get<1>`, `std::get<2>` to verify
the contents.

## Run

```sh
./verify.sh
```

## Hints

- One line:
  ```cpp
  return record(id, std::move(name), score);
  ```
  `std::move(name)` avoids copying the string when constructing
  the tuple. The tuple's first parameter is `int` (cheap),
  third is `double` (cheap); only the string benefits.

- Or use brace-init:
  ```cpp
  return record{id, std::move(name), score};
  ```
  Functionally equivalent; the brace-init form is shorter in
  some readers' minds.

- `std::tuple` lives in `<tuple>`. `std::string` in `<string>`.

- Tuple element access is *positional* and *zero-indexed*.
  Index out-of-range is a compile error (template
  instantiation failure), not a runtime crash.

- The element type at position I in `tuple<T0, T1, T2, ...>` is
  `std::tuple_element<I, T>::type`. The size of the tuple is
  `std::tuple_size<T>::value`. Both are template
  metafunctions; we don't use them here, but they're the
  introspection primitives if you ever need them.

- Compared to a pair: `tuple<T1, T2>` is the N-arity
  generalisation. `pair` is older, has named members
  (`.first`, `.second`), and integrates with `std::map`. A
  tuple-of-two is *not* implicitly convertible to a pair (they
  are different types).

- Steps 99 and 100 cover the rest of the tuple surface area
  (`std::tie`, structured-binding-via-tie, `make_tuple`,
  `forward_as_tuple`). This step is just "how do you make
  one".
