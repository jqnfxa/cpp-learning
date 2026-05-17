# 49 — Captures by value `[x]`

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

A capture list `[ ... ]` tells the lambda *what to remember from
the enclosing scope*. The simplest form is **capture by value**:

```cpp
int outer = 10;
auto get_it = [outer]() { return outer; };
outer = 100;
int v = get_it();      /* still 10 -- captured a snapshot */
```

Two things to know:

1. **A by-value capture is a *snapshot***. The lambda stores its
   own copy at the moment the lambda expression is evaluated.
   Subsequent changes to the outer variable don't affect the
   stored copy.

2. **By-value captures are `const` inside the lambda.** You can
   read them; you can't modify the stored copy unless you mark
   the lambda `mutable` (step 52). Trying to assign to a
   by-value capture in a non-mutable lambda is a compile error.

The third thing (mostly for the next step): there's also
**capture by reference** (`[&outer]`), which is *not* a snapshot
— the lambda holds a reference into the enclosing scope.

## Task

In `solution.hpp`, implement two functions that *return*
lambdas via `std::function`:

```cpp
/* Returns a callable that always returns `value`. */
std::function<int()> make_constant(int value);

/* Returns a callable that adds `delta` to its argument. */
std::function<int(int)> make_adder(int delta);
```

Both should use **`[value]`** / **`[delta]`** capture-by-value.
The returned `std::function` outlives the call to
`make_constant` / `make_adder`, so capturing by reference to the
parameter would be a use-after-free.

| Call                                | Returns |
|-------------------------------------|---------|
| `make_constant(42)()`               | `42`    |
| `make_constant(0)()`                | `0`     |
| `make_adder(5)(3)`                  | `8`     |
| `make_adder(0)(100)`                | `100`   |
| `make_adder(-1)(10)`                | `9`     |

## Run

```sh
./verify.sh
```

## Hints

- Bodies:
  ```cpp
  std::function<int()> make_constant(int value)
  {
      return [value]() { return value; };
  }

  std::function<int(int)> make_adder(int delta)
  {
      return [delta](int n) { return n + delta; };
  }
  ```

- **Don't** capture by reference here:
  ```cpp
  return [&value]() { return value; };   /* BUG: dangling reference */
  ```
  `value` is a local parameter that goes out of scope when
  `make_constant` returns. The lambda would hold a reference to
  deallocated memory.

- `std::function<R(Args...)>` lives in `<functional>`. It's a
  type-erased wrapper around any callable with the matching
  signature.

- The tests include a "snapshot" test that mutates an outer
  variable after capturing and verifies the lambda's stored copy
  is unaffected. Your by-value capture is what makes that work.
