# 95 — `std::bind`

> Spec: <https://en.cppreference.com/cpp/utility/functional/bind>

## What you learn

`std::bind` (in `<functional>`) creates a callable that wraps
another callable with some arguments **pre-filled** and others
**placeheld** for the eventual call. The placeholders live in
`std::placeholders` and are named `_1`, `_2`, ... — they
represent "argument position N at the call site".

```cpp
using namespace std::placeholders;

int add(int a, int b) { return a + b; }

auto add_to_10 = std::bind(add, 10, _1);
add_to_10(5);   /* -> add(10, 5) == 15 */

auto swap_args = std::bind(add, _2, _1);
swap_args(3, 7); /* -> add(7, 3) == 10  (placeholders reordered) */
```

The result is a callable object. Most often you assign it into
a `std::function` for storage, but you can also invoke it
directly.

### Member functions

`std::bind` is the original way to make a callable from a
**member function pointer** + an object:

```cpp
struct Adder
{
    int base;
    int add(int x) const { return base + x; }
};

Adder a{10};
auto bound = std::bind(&Adder::add, &a, _1);
bound(5);   /* calls a.add(5) -> 15 */
```

The first argument to `std::bind` is the member function
pointer. The second is the implicit `this` (here, `&a`). The
remaining arguments fill the method's parameters. With `_1`
for the parameter, you get back a `function<int(int)>`-like
callable.

The same pattern with C++11 lambdas:

```cpp
auto bound = [&a](int x) { return a.add(x); };
```

The lambda version is almost always preferred in modern code —
shorter, more readable, captures clear. `std::bind` survives in
codebases for two reasons:
- Compatibility with C++03/C++11 code that pre-dated lambdas
  becoming idiomatic.
- The placeholder-reordering form (`_2, _1`) is occasionally
  shorter than the equivalent lambda.

### Argument capture vs reference

`std::bind` copies its arguments by default. To pass by
reference, wrap with `std::ref` or `std::cref`:

```cpp
int counter = 0;
auto inc = std::bind([](int &n) { ++n; }, std::ref(counter));
inc();   /* counter is now 1 */
inc();   /* counter is now 2 */
```

Without `std::ref`, the captured `counter` would be a copy and
each call would increment a different (internal) integer.

### The "use lambdas" recommendation

Effective Modern C++ Item 34 makes the case: prefer lambdas
over `std::bind`. The reasons:
- Lambdas are easier to read.
- Lambdas inline; `bind` has the type-erasure overhead even
  with a non-`std::function` storage path.
- Lambdas type-check arguments at construction; `bind`
  type-checks at *call*.

For this step, the task uses `std::bind` deliberately, because
*knowing* the tool is part of the C++11 curriculum — even when
you'd reach for a lambda in practice.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task95
{
    struct Adder
    {
        int base;
        int add(int x) const;
    };

    /* Return a std::function<int(int)> equivalent to
     *   [&a](int x) { return a.add(x); }
     * built with std::bind(&Adder::add, &a, _1). */
    std::function<int(int)> bind_member(const Adder &a);

    /* Return a std::function<int(int)> equivalent to
     *   [base](int x) { return base + x; }
     * built with std::bind on a free addition function (or a lambda
     * captured via bind). For this task, bind a free `add(int, int)`
     * with `base` as the first argument and `_1` as the second. */
    std::function<int(int)> bind_free(int base);
}
```

`Adder::add(x)` returns `base + x`.

## Run

```sh
./verify.sh
```

## Hints

- Bring in placeholders:
  ```cpp
  using std::placeholders::_1;
  ```
  Or qualify each one as `std::placeholders::_1`.

- `bind_member` body:
  ```cpp
  using std::placeholders::_1;
  return std::bind(&Adder::add, &a, _1);
  ```

- `bind_free` needs a free function to bind. The cleanest way
  inside the solution header is to define a private helper:
  ```cpp
  namespace detail { inline int add_ints(int a, int b) { return a + b; } }
  ```
  Then `return std::bind(detail::add_ints, base, _1);`.

- The `&a` you bind to `Adder::add` is a pointer to a
  caller-supplied lvalue. The pointed-to object must outlive
  the bound callable. The tests handle this by keeping the
  Adder alive for the duration of the callable's use.

- A long-form alternative is `std::bind(&Adder::add,
  std::ref(a), _1)`. Both work; `&a` is shorter and equally
  correct.

- Don't capture `a` by *value* into the bind. `std::bind(...,
  a, ...)` would *copy* the Adder into the bind object. The
  member-pointer form takes `Adder *` (or an Adder reference
  wrapper), which is the lighter and idiomatic version.

- C++17 added `std::invoke` which is the lower-level primitive
  that `std::bind` builds on. C++20's ranges and projections
  also use the "INVOKE expression" semantics. Knowing
  `std::bind` makes the rest of the modernised
  callable-handling ecosystem familiar.
