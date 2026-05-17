# 54 — Lambda as a `std::function`

> Spec: <https://en.cppreference.com/cpp/utility/functional/function>

## What you learn

`std::function<R(Args...)>` is a **type-erased wrapper** around any
callable with a matching signature: a free function pointer, a
member-function pointer, a lambda (captureless or capturing!), or
a hand-rolled functor.

```cpp
std::function<int(int)> f;

f = [](int x) { return x + 1; };           /* captureless lambda */
f = std::function<int(int)>([](int x) { return x + 1; });
int delta = 10;
f = [delta](int x) { return x + delta; };  /* capturing lambda */
f = &::strlen;                              /* close enough; signature must match exactly */
```

The point: in step 53 you converted a captureless lambda to a
function pointer. Captureless lambdas have a unique anonymous
type, and once you assign one to `int(*)(int)` you lose that type
identity — fine for C APIs. But:

- **`std::function` accepts *capturing* lambdas** that have no
  fnptr conversion at all.
- **`std::function` is heterogeneous storage**: a
  `std::vector<std::function<...>>` can hold many different lambda
  types as long as their call signatures match. With raw lambdas,
  each has its own type and you'd need separate variables.

The trade-off is **runtime overhead**: storing a callable in
`std::function` is a small-buffer-optimized type-erased
wrapper. There's a one-time setup cost and an indirect call when
you invoke it. For hot loops, prefer the raw lambda; for storage,
collections, and configuration, `std::function` is the right tool.

## Task

In `solution.hpp`, implement:

```cpp
/* Returns a callable that multiplies its argument by `factor`. */
std::function<int(int)> make_multiplier(int factor);

/* Walk a chain of callbacks: x = fns[0](x); x = fns[1](x); ... */
int apply_all(const std::vector<std::function<int(int)>> &fns, int x);
```

| Operation                                              | Returns |
|--------------------------------------------------------|---------|
| `make_multiplier(3)(5)`                                | `15`    |
| `make_multiplier(0)(99)`                               | `0`     |
| `apply_all({}, 10)`                                    | `10`    |
| `apply_all({make_multiplier(2)}, 10)`                  | `20`    |
| `apply_all({make_multiplier(2), make_multiplier(3)}, 10)` | `60` (10*2=20, 20*3=60) |
| `apply_all({add1, mul2}, 10)`                          | `22` (10+1=11, 11*2=22) |

The crucial point: the vector contains `std::function` slots, so
it can hold *any* callable matching `int(int)` — a capturing
`make_multiplier` lambda alongside an inline captureless `[](int
x) { return x + 1; }`.

## Run

```sh
./verify.sh
```

## Hints

- `make_multiplier`:
  ```cpp
  std::function<int(int)> make_multiplier(int factor)
  {
      return [factor](int x) { return x * factor; };
  }
  ```

- `apply_all`:
  ```cpp
  int apply_all(const std::vector<std::function<int(int)>> &fns, int x)
  {
      int result = x;
      for (const auto &f : fns) result = f(result);
      return result;
  }
  ```

- Initializing a `std::vector<std::function<int(int)>>` from a
  brace-list of mixed callables (some capturing, some not) is
  the headline test case. Each element is implicitly constructed
  into a `std::function`.

- The `std::function` slot can be empty (default-constructed),
  but calling an empty one throws `std::bad_function_call`. Tests
  here always populate the vector before iterating.

- Include `<functional>` for `std::function` and `<vector>` for
  `std::vector`.
