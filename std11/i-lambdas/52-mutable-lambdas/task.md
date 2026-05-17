# 52 — `mutable` lambdas

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

A lambda's `operator()` is **`const` by default**. That means:
- By-value captures are read-only inside the body.
- A by-value capture of a `T` looks like a `const T` member.

The **`mutable`** keyword removes this implicit `const`:

```cpp
[capture](params) mutable -> return-type { body }
```

Now `operator()` is non-`const` and you can modify the by-value
captures. This is how you build a **stateful lambda** — a callable
that remembers something across calls.

### The classic counter

```cpp
int start = 0;
auto next = [start]() mutable {
    return ++start;       /* mutates the lambda's own copy of `start` */
};

next();    /* returns 1 -- lambda's `start` is now 1 */
next();    /* returns 2 */
next();    /* returns 3 */
/* The *outer* `start` is still 0. The lambda has its own copy. */
```

The lambda's body sees its **own** `start`, copied at lambda
creation. Each call mutates that copy. The original outer `start`
is unaffected (it was captured by value, after all).

### Where `mutable` doesn't matter

For **by-reference** captures (`[&x]`), there's no implicit const
— you can modify `x` directly through the reference, no `mutable`
needed. `mutable` only matters for by-value captures.

C++14 will introduce **init-captures** (`[count = 0]`), which let
you create a new "variable" inside the lambda without an outer
binding. That's the more modern idiom for stateful lambdas — but
in C++11 you have to capture an existing variable by value and
mark `mutable`.

## Task

In `solution.hpp`, implement:

```cpp
/* Returns a counter that yields `start + 1`, `start + 2`, ...
 * on successive calls. */
std::function<int()> make_counter(int start);
```

| Sequence              | Output                  |
|-----------------------|-------------------------|
| `make_counter(0)`     | `1, 2, 3, 4, 5, ...`    |
| `make_counter(10)`    | `11, 12, 13, ...`       |
| `make_counter(-5)`    | `-4, -3, -2, ...`       |

Each `make_counter` call should produce an *independent* counter:
two counters made with the same `start` produce the same sequence
but don't share state.

## Run

```sh
./verify.sh
```

## Hints

- The body:
  ```cpp
  std::function<int()> make_counter(int start) {
      return [start]() mutable { return ++start; };
  }
  ```

- `++start` returns the *new* value (pre-increment). Use that for
  the test sequence `1, 2, 3, ...` starting from `start = 0`.

- Why does this even work? The captured `start` inside the lambda
  is stored as a member of the closure type. Marking the lambda
  `mutable` makes the auto-generated `operator()` non-const, so
  you can mutate that member.

- A common gotcha: forgetting `mutable`. The compiler error
  reads "assignment of read-only member ..." or "increment of
  read-only member ..." — pointing at the captured variable
  inside the lambda body.

- Don't capture `start` by reference (`[&start]`):
  ```cpp
  return [&start]() mutable { return ++start; };   /* BAD */
  ```
  `start` is a parameter. The returned lambda would have a
  dangling reference once `make_counter` returns.
