# 94 — `std::function`

> Spec: <https://en.cppreference.com/cpp/utility/functional/function>

## What you learn

`std::function<R(Args...)>` is a **type-erased function
wrapper**. It can hold:

- A function pointer (`int (*)(int)`).
- A lambda (with or without captures).
- A functor (a class with `operator()`).
- The result of `std::bind`.
- Anything else callable with the right signature.

All four of those have different types as far as the language is
concerned, but a `std::function<int(int)>` flattens them into one
uniform type that can be stored in a container, passed around,
and re-assigned at runtime.

```cpp
std::function<int(int)> f;
f = [](int x) { return x * 2; };       /* captureless lambda */
f = +[](int x) { return x * 2; };      /* function pointer */
f = std::negate<int>();                 /* functor from <functional> */

int captured = 100;
f = [captured](int x) { return x + captured; };  /* capturing lambda */

f(5);                                   /* dispatches to whatever's stored */
```

### How it does it

Type erasure: internally, `std::function` stores a polymorphic
wrapper around the callable, plus a small-buffer optimisation
for small targets (typically up to 16 or 24 bytes -- enough for
a function pointer + a couple of capture words). Larger targets
heap-allocate.

The trade-offs vs a templated callable parameter:

| Property                          | `std::function<int(int)>` | `template <class F> void use(F)` |
|-----------------------------------|----------------------------|-----------------------------------|
| Stored in a container?            | yes                        | no (different `F` per call)       |
| Re-assignable at runtime?         | yes                        | no                                |
| Call cost                         | virtual-like dispatch       | inlined                           |
| Holds captures?                   | yes                        | yes                               |

Use `std::function` when you need *uniformity* (containers,
re-assignment). Use templates when you need *speed*
(inlining).

### The dispatch table pattern

The classic use of `std::function` is a name-keyed dispatch
table -- the C++ equivalent of a switch on operation names:

```cpp
std::unordered_map<std::string, std::function<int(int)>> ops = {
    {"negate",  [](int x) { return -x; }},
    {"double",  [](int x) { return x * 2; }},
    {"square",  [](int x) { return x * x; }},
};

int result = ops.at("negate")(7);   /* -7 */
```

Each value in the map is a completely different lambda type at
the language level, but they all fit into the same
`std::function<int(int)>` slot. Without `std::function`, you'd
have to hand-roll an inheritance hierarchy or use raw function
pointers (which can't hold captures).

### Empty `std::function`

A default-constructed `std::function` holds nothing. Calling it
throws `std::bad_function_call`. Check with `operator bool()`:

```cpp
std::function<int(int)> f;
if (f) f(5);
else /* not set yet */;
```

This is one of the few standard-library types that has a
*specific empty state* — most "default-constructed" types are
fully usable.

## Task

In `solution.hpp`, implement a dispatch table:

```cpp
namespace task94
{
    using op             = std::function<int(int)>;
    using dispatch_table = std::unordered_map<std::string, op>;

    /* Return a table populated with these operations:
     *   "negate" -> -x
     *   "double" -> 2 * x
     *   "square" -> x * x
     *   "succ"   -> x + 1
     *   "pred"   -> x - 1
     */
    dispatch_table make_arithmetic_table();

    /* Look up `name` in t and call the stored function with x.
     * Throws std::out_of_range if the name is not present. */
    int apply(const dispatch_table &t, const std::string &name, int x);
}
```

## Run

```sh
./verify.sh
```

## Hints

- For `make_arithmetic_table`, use brace-init with five
  key-lambda pairs. Each lambda is `[](int x) { return ...; }`.

- For `apply`, `t.at(name)(x)` does both lookups in one line:
  `at` throws `std::out_of_range` on a missing key (exactly the
  required behaviour), and `operator()` invokes the stored
  function. No manual exception handling needed.

- Capturing lambdas would also work for stateful operations
  (e.g. `[k](int x) { return x + k; }`). The task uses only
  captureless lambdas; both compile to the same
  `std::function<int(int)>` slot.

- `std::function` lives in `<functional>`. Include it.

- For the empty-name test, the task says "throws
  `std::out_of_range`". `unordered_map::at` throws exactly that;
  `operator[]` would *insert* a default-constructed
  `std::function` and then call it (which throws
  `bad_function_call`). Use `at`, not `[]`.

- Performance note for real code: every `std::function` call has
  a small fixed overhead (an indirect call). For a hot loop with
  one known callable, switch to a template parameter; for a
  one-call-per-request dispatch table, `std::function` is fine.

- C++23's `std::function_ref` (and the earlier `tl::function_ref`
  library) is a non-owning version for cases where you don't
  need to store the callable past the call. Not part of this
  task; mentioned for future awareness.
