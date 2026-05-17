# 12 — `std::make_unique`

> Spec: <https://en.cppreference.com/cpp/memory/make_unique>

## What you learn

C++11 shipped `std::make_shared` but not its `unique_ptr`
sibling -- an obvious oversight that C++14 corrected.
`std::make_unique<T>(args...)` creates a `unique_ptr<T>` by
perfect-forwarding `args...` to `T`'s constructor:

```cpp
auto p = std::make_unique<widget>(42);    /* widget(42) -- unique_ptr<widget> */
```

Equivalent to the C++11 incantation:

```cpp
std::unique_ptr<widget> p(new widget(42));
```

but shorter, doesn't repeat the type, and (the main benefit)
exception-safe when used inside a compound expression.

### Why this matters: the compound-expression leak

The classic C++11 hazard:

```cpp
do_thing(
    std::unique_ptr<A>(new A),
    std::unique_ptr<B>(new B));      /* DANGER */
```

The C++14 standard is silent on the order in which the
arguments evaluate. A typical compiler might:

1. Call `new A` -> raw pointer `pA`.
2. Call `new B` -> raw pointer `pB`.
3. Construct `unique_ptr<A>(pA)`.
4. Construct `unique_ptr<B>(pB)`.

If step 2 throws (allocation failure, B's constructor
throws), `pA` is leaked -- it's a raw pointer, no
destructor will free it.

With `make_unique`:

```cpp
do_thing(
    std::make_unique<A>(),
    std::make_unique<B>());          /* SAFE */
```

Each `make_unique` call atomically packages the raw `new`
with the `unique_ptr` constructor. Any exception thrown
after the `new` and before the `unique_ptr` ownership is
handled by `make_unique`'s implementation -- a `new` that
throws can never leak.

C++17 strengthened the evaluation-order rules so that the
naïve form is also safe, but for C++14 (and for older C++17
compilers), `make_unique` is the only portable cure.

### The shape

```cpp
template <class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args);          /* (1) object form */

template <class T>                                       /* (2) array form */
std::unique_ptr<T> make_unique(std::size_t n);
   /* enabled when T is U[] (unbounded array) */

template <class T, class... Args>                        /* (3) DELETED */
/* deleted */ make_unique(Args&&...);
   /* disabled when T is U[N] (bounded array) -- can't return unique_ptr<U[N]> */
```

Three overloads, two usable:

1. **Object form**: forwards args to T's constructor. The
   common case.

2. **Array form**: takes a single size, value-initialises all
   elements (so `int[N]` is zeroed). No way to pass an
   initialiser-list per element in C++14.

3. The bounded-array form is deleted -- `unique_ptr<int[5]>`
   doesn't make sense (you'd use `std::array` or
   `std::unique_ptr<int[]>`).

### Object form: pass any constructor args

```cpp
auto a = std::make_unique<widget>(42);
auto b = std::make_unique<widget>(some_int_var);
auto c = std::make_unique<std::string>("hello");
auto d = std::make_unique<std::string>(5, 'x');    /* "xxxxx" */
auto e = std::make_unique<std::pair<int, double>>(1, 2.5);
```

The forwarding goes through `std::forward<Args>(args)...`,
so lvalues stay lvalues and rvalues stay rvalues.

### Array form: just the size

```cpp
auto arr = std::make_unique<int[]>(100);    /* int[100], all zero */

arr[42] = 99;                                /* indexable like int* */
```

`unique_ptr<int[]>` deletes with `delete[]` (the array form),
not `delete`. Don't try `unique_ptr<int>` for an array
allocation -- that mismatches and is UB.

The C++14 array form **value-initialises** the elements:
arithmetic types are zero, user types get their default
constructor. If you want to skip initialisation for raw
buffers, use `new` directly (or C++20's
`std::make_unique_for_overwrite`).

### make_unique vs make_shared

| Feature                   | `make_unique`      | `make_shared`      |
|---------------------------|--------------------|--------------------|
| One allocation            | yes                | yes (control block + T fused) |
| Forwards ctor args        | yes                | yes                |
| Object form               | yes                | yes                |
| Array form                | yes (T[])          | C++20+             |
| Custom deleter            | no                 | yes (via overload) |
| Custom allocator          | no                 | `allocate_shared`  |

For "I want to own this object exclusively, no shared
ownership," `make_unique`. For "shared ownership, maybe with
weak_ptr later," `make_shared`. Don't use `make_shared` just
because it's "newer" -- the shared overhead is wasted for
single-owner cases.

### Custom deleter

`make_unique` does NOT support custom deleters. The
`unique_ptr<T, Deleter>` types with a non-default deleter
have to be constructed manually:

```cpp
std::unique_ptr<FILE, decltype(&std::fclose)> file(
    std::fopen("x.txt", "r"), &std::fclose);
```

There's no `make_unique` overload for this. The reason:
custom-deleter types are diverse enough that a uniform
factory would either be very generic or very limited; the
committee opted to leave it out.

### Naming convention

In real code, you'll often see:

- `std::unique_ptr<T> p = std::make_unique<T>(args);` --
  explicit type.
- `auto p = std::make_unique<T>(args);` -- type from RHS.

The `auto` form is cleaner and matches the spirit of "don't
repeat the type." Use `auto` unless you need a specific
qualifier (`std::shared_ptr<const T> p =
std::make_shared<T>(args);` -- the type widens).

## Task

In `solution.hpp`, fill in three function bodies:

```cpp
inline std::unique_ptr<widget> make_widget(int value) {
    return std::make_unique<widget>(value);
}

inline std::unique_ptr<std::string> make_string_n(std::size_t count, char c) {
    return std::make_unique<std::string>(count, c);
}

inline std::unique_ptr<int[]> make_array(std::size_t size) {
    return std::make_unique<int[]>(size);
}
```

The first two use the **object form** (forward ctor args).
The third uses the **array form** (single size).

## Run

```sh
./verify.sh
```

## Hints

- Each body is a single `return std::make_unique<T>(args)`
  expression.

- `widget`'s ctor takes one `int`; just forward `value`.

- `std::string` has a `(size_t, char)` ctor that builds a
  string of `count` copies of `c`. `make_unique` forwards
  both args.

- The array form `std::make_unique<int[]>(size)` takes the
  size and value-initialises -- every int is 0.

- Don't write `std::unique_ptr<T>(new T(args))` -- that's
  the C++11 form. The whole point of this step is to use
  `make_unique`.

- Don't add a custom deleter; `make_unique` doesn't support
  one. The functions all use the default deleter.

- Tests verify: each function returns a non-null pointer
  with the right value; two consecutive calls produce
  distinct pointers (unique storage); array form is
  zero-initialised; ownership transfer via std::move
  leaves the source empty.

- C++17 added `std::optional`, `std::variant`, and many
  things; C++20 added `std::make_unique_for_overwrite` for
  the "skip default-init" case. For C++14, `make_unique` is
  the single tool for "construct + wrap in unique_ptr."
