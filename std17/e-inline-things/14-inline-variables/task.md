# 14 — `inline` variables

> Spec: <https://en.cppreference.com/cpp/language/inline>

## What you learn

C++17 added `inline` for **variables** (it already existed
for functions). An `inline` namespace-scope variable can be
defined in a header and INCLUDED in many TUs without
multiple-definition errors:

```cpp
/* header.hpp */
namespace ns {
    inline int counter = 0;            /* OK: shared definition */
    inline const int max_retries = 3;
    inline double pi = 3.14159;
}
```

All TUs see the **same** `counter` variable. The C++14 fix
required:

- Declaring `extern int counter;` in the header.
- Defining `int counter = 0;` in one specific `.cpp` file.

`inline` collapses this to a single declaration-definition
in the header.

### vs `static`

```cpp
static int counter = 0;     /* internal linkage: PER-TU copy */
inline int counter = 0;     /* external linkage: ONE shared copy */
```

The difference is invisible in a single-TU build but
critical in multi-TU programs:

- `static`: each TU has its own `counter`. Incrementing
  from one TU doesn't affect the other.
- `inline`: all TUs share one `counter`. Increments are
  visible everywhere.

The test exercises this by adding a second TU
(`helper.cpp`) that touches the same variables. The
"cross_tu_increment_chain" and "helper_sees_same_counter"
tests are the multi-TU detector.

### When to use it

- **Header-only constants** that should be shared across
  TUs: `inline constexpr int MAX = 100;`
- **Header-only counters / stats** for testing or
  instrumentation.
- **Global registry tables** populated by static
  initializers in headers.

For type-level constants (e.g., `static constexpr int N = 5;`
inside a class), `inline` lets you remove the
out-of-class definition that pre-C++17 required.

### Static members

C++17 also made `static constexpr` data members of classes
**implicitly inline**:

```cpp
struct config {
    static constexpr int max_retries = 3;   /* C++17: implicitly inline */
};
/* No `int config::max_retries;` definition needed in any .cpp */
```

This was a huge cleanup -- pre-C++17 codebases had a
boilerplate `int config::max_retries;` line in some random
`.cpp` per static-constexpr member.

## Task

In `solution.hpp`, change `static` to `inline` on three
variables. The `helper.cpp` TU shares the same definitions
via the include.

## Run

```sh
./verify.sh
```

## Hints

- `inline int global_counter = 0;` -- the C++17 form.

- `static int global_counter = 0;` in a header gives
  PER-TU copies. The cross-TU tests detect this.

- For const variables in a header, `inline const int x =
  ...` (or `inline constexpr int x = ...`) is the
  canonical C++17 form.

- `inline` doesn't make the variable read-only;
  combine with `const` or `constexpr` if you want that.

- Tests verify: per-TU access matches, max_retries / pi
  values, cross-TU counter sharing (the KEY tests --
  helper.cpp's view of `global_counter` matches
  tests.cpp's).

- Multi-file test setup: `helper.cpp` is compiled and linked
  with `tests.cpp` (see CMakeLists). It accesses the same
  symbols.

- C++17 inline also applies to static data members and
  variable templates -- same single-definition guarantee.
