# 5 — Generic lambdas (`auto` parameters)

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

C++14 added `auto` as a permitted parameter type in lambdas.
The lambda's call operator becomes a **template** instead of
a single non-template function:

```cpp
auto square = [](auto const &x) { return x * x; };

square(5);        /* operator()<int>:    int    -> int    */
square(2.5);      /* operator()<double>: double -> double */
square(7L);       /* operator()<long>:   long   -> long   */
```

Same lambda object; each call site instantiates a fresh
specialisation of the closure's `operator()`. This is the
lambda equivalent of a function template, without the
verbose template-parameter declaration.

### Equivalent template

Conceptually, the generic lambda above expands to a closure
type like:

```cpp
struct __square_closure
{
    template <class T>
    auto operator()(T const &x) const { return x * x; }
};
__square_closure square{};
```

The struct itself isn't a template; the call operator is.
Each call to `square(arg)` deduces `T` from `arg` and
instantiates one operator() specialization. That's why the
same closure can handle `int`, `double`, `std::string`, etc.

### Heterogeneous arguments

A generic binary lambda can take two DIFFERENT types:

```cpp
auto add = [](auto const &a, auto const &b) { return a + b; };

add(1, 2.5);              /* (int, double)  -> double */
add(std::string{"x"}, "y"); /* (string, const char*) -> string */
```

Each parameter gets its own deduced type. The return type
(via `auto` return deduction from step 3) follows
`decltype(a + b)`.

### Why this matters

Before C++14, you had two options for "generic" lambdas:

1. Write a full function template -- which is fine, but
   doesn't compose with the rest of your lambda-heavy code:

   ```cpp
   template <class T> T square(T const &x) { return x * x; }
   std::transform(v.begin(), v.end(), v.begin(), square<int>);
   ```

2. Write a lambda for one specific type and duplicate it for
   others:

   ```cpp
   auto sq_int    = [](int x)    { return x * x; };
   auto sq_double = [](double x) { return x * x; };
   ```

C++14 collapses both into one ergonomic line. STL algorithms
benefit enormously -- `std::transform`, `std::for_each`,
`std::sort` all accept any callable, and a single generic
lambda can drive them across all element types in your
program.

### Namespace-scope generic lambdas in C++14

To define a generic lambda at namespace scope in a HEADER,
C++14 requires either:

1. `static auto name = [](...){...};` -- each TU gets its own
   copy, no ODR risk.
2. A function returning the lambda:
   ```cpp
   inline auto make_square() { return [](auto x){ return x*x; }; }
   ```
3. Define it inside a single .cpp file with external linkage.

C++17 added **inline variables**, so you can write
`inline auto square = [](auto x){ return x*x; };` in a header
and get one shared definition across TUs. For C++14 the
`static` form is the most ergonomic.

### Generic lambdas inside containers and adapters

```cpp
auto by_length = [](auto const &a, auto const &b) {
    return a.size() < b.size();
};

std::vector<std::string> v = {"long", "tiny"};
std::sort(v.begin(), v.end(), by_length);
```

The lambda doesn't care about element type -- as long as
`.size()` is defined, the call site picks the right
operator() instantiation.

### Pre-C++17 limitations

Generic lambdas in C++14 are:

- **Not `constexpr`-callable**: the call operator isn't
  implicitly `constexpr` until C++17. So you can't use a
  generic lambda's result in a constant expression in C++14.
- **Not `noexcept`-deducing**: the lambda doesn't deduce
  noexcept from its body until C++17 either.
- **No template parameter clause**: you can't write
  `[]<class T>(T x){...}` until C++20.

For C++14, the `auto`-parameter form is the only generic
form. Step 6 (init-capture) adds the other piece of C++14's
lambda story.

### Performance

Generic lambdas compile to the same code as a hand-rolled
template function. The closure type has zero overhead; the
template instantiation cost is paid at compile time, not
runtime. Inlining works the same as for any small function.

## Task

In `solution.hpp`, fill in the two generic lambda bodies:

- `square`: takes one `auto const &x`, returns `x * x`.
- `concat`: takes `auto const &a` and `auto const &b`,
  returns `a + b`.

```cpp
namespace task05
{
    static auto square = [](auto const &x)             { return x * x; };
    static auto concat = [](auto const &a, auto const &b) { return a + b; };
}
```

## Run

```sh
./verify.sh
```

## Hints

- Each body is one expression. Don't overthink it.

- The lambdas are declared `static auto` at namespace scope
  -- the C++14-friendly form for header-only generic
  lambdas. Don't change the declaration; only fill in the
  body's return expression.

- `square(5)` -> 25 (int * int). `square(2.5)` -> 6.25
  (double * double). The compiler instantiates one
  operator() per element type.

- `concat(1, 2)` -> 3 (int+int). `concat(1, 2.5)` -> 3.5
  (int + double = double; the return type follows decltype).
  `concat(string, string)` -> string. `concat(string, const
  char*)` -> string.

- The "multiple specialisations coexist" test calls each
  lambda with multiple element types in one TEST -- proves
  that the same closure object handles many types.

- Tests use runtime `EXPECT_TRUE(std::is_same<...>::value)`
  for type checks, not `static_assert`, so the stub stays
  compilable.

- A generic lambda can be passed to STL algorithms
  (`std::transform`, `std::sort`, ...) without naming
  template parameters -- that's the everyday payoff. We don't
  exercise that here, but it's the reason you'll write
  generic lambdas in real code.
