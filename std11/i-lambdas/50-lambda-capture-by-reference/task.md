# 50 — Captures by reference `[&x]`

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

The other primary capture form is **by reference**: `[&x]`. Unlike
the snapshot semantics of `[x]` (step 49), `[&x]` makes the lambda
hold a *reference* to the outer variable. Reads see the current
value; writes propagate back.

```cpp
int counter = 0;
auto increment = [&counter]() { ++counter; };

increment();
increment();
increment();
/* counter is now 3 */
```

Two consequences:

1. **The variable can be mutated.** No `mutable` keyword required
   — the lambda isn't modifying *its own* state, it's modifying
   the *outer* binding through a reference.

2. **The outer variable must outlive the lambda.** If the lambda
   is stored (in a `std::function`, in a container, returned from
   a function) past the lifetime of the captured variable, you
   have a dangling reference. This is the dual of step 49's
   "by-value capture survives the original."

## When to use which

| Need                                | Capture with |
|-------------------------------------|--------------|
| Snapshot the value at lambda creation | `[x]`        |
| Lambda outlives the caller's scope   | `[x]`        |
| Mutate the outer variable            | `[&x]`       |
| Update aggregate state during iteration | `[&x]`    |

The common case where `[&]` shines: passing a lambda to an
algorithm that walks a range while accumulating into a shared
variable.

## Task

In `solution.hpp`, implement two functions, each using a lambda
that captures **by reference**:

```cpp
/* Initialize a counter to 0, run an internal lambda that
 * increments it `n` times via `&counter` capture, return the
 * counter's value. */
int count_up_to(int n);

/* Sum the elements of arr[0..n) using a lambda captured `&total`
 * inside a range-walk. */
int sum_via_reference_capture(const int *arr, int n);
```

| Call                            | Returns |
|---------------------------------|---------|
| `count_up_to(0)`                | `0`     |
| `count_up_to(1)`                | `1`     |
| `count_up_to(5)`                | `5`     |
| `count_up_to(100)`              | `100`   |
| `sum_via_reference_capture({}, 0)` | `0`  |
| `sum_via_reference_capture({1,2,3,4,5}, 5)` | `15` |
| `sum_via_reference_capture({-3,5,-2,4}, 4)` | `4` |

## Run

```sh
./verify.sh
```

## Hints

- `count_up_to`:
  ```cpp
  int count_up_to(int n)
  {
      int counter = 0;
      auto inc = [&counter]() { ++counter; };
      for (int i = 0; i < n; ++i)
          inc();
      return counter;
  }
  ```

- `sum_via_reference_capture`:
  ```cpp
  int sum_via_reference_capture(const int *arr, int n)
  {
      int total = 0;
      auto add = [&total](int x) { total += x; };
      for (int i = 0; i < n; ++i)
          add(arr[i]);
      return total;
  }
  ```

- These could be written with a plain `for` loop and no lambda
  at all — that's the right call in real code. The point of this
  step is to *practice* the by-reference capture form. In real
  use, by-reference capture commonly appears with algorithm
  templates like `std::for_each` (step 80).

- Don't accidentally write `[counter]` (by-value): the lambda
  would have a `const` snapshot of the value and `++counter`
  wouldn't compile.

- Don't return the lambda from the function — `counter` /
  `total` are stack variables that die when the function returns.
  This is the use case where by-reference capture *doesn't*
  apply.
