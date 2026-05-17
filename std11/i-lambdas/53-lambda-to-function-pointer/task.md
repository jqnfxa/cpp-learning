# 53 — Captureless lambda → function pointer

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

A **captureless** lambda — one with an empty capture list `[]` —
has an extra property the standard grants no other lambda: an
implicit conversion to a plain C-style **function pointer**.

```cpp
int (*fn)(int) = [](int x) { return x * 2; };
int y = fn(5);     /* 10 */
```

The lambda's anonymous closure type provides a conversion
operator: `closure_type::operator R(*)(Args...)() const`. The
compiler emits a free function with the right signature and the
conversion returns its address.

This matters because **C APIs take function pointers**, not
function objects. If you want to pass a callback to `qsort`,
`pthread_create`, GLFW, or any of a thousand C libraries, you can
write the callback as a captureless lambda and pass it directly:

```cpp
std::qsort(arr, n, sizeof(int),
           [](const void *a, const void *b) {
               return *(const int *)a - *(const int *)b;
           });
```

No need to declare a static free function.

## The capturing-lambda limitation

A lambda **with** captures cannot convert to a function pointer.
Its closure object needs to *carry state* (the captured values),
which a bare function pointer cannot. If you need to pass a
stateful callable through a function-pointer API, you typically
need a "userdata pointer" trampoline (a separate `void *` arg
that points to your state), or you use `std::function` for higher
levels.

## Task

In `solution.hpp`, implement two functions:

```cpp
/* Return a function pointer to a "doubler": fn(x) -> x * 2. */
int (*get_doubler())(int);

/* Take a function-pointer callback and apply it to x. */
int apply(int (*fn)(int), int x);
```

The trick is that `get_doubler` returns a *function pointer*, not
a lambda type, so the body uses the implicit conversion:

```cpp
inline int (*get_doubler())(int)
{
    return [](int x) { return x * 2; };
    /*     ^^^^^^^^^^^^^^^^^^^^^^^^^^^ captureless lambda */
    /*     implicitly converts to int(*)(int) */
}
```

`apply` just calls through the pointer.

| Call                                     | Returns |
|------------------------------------------|---------|
| `get_doubler()(3)`                       | `6`     |
| `get_doubler()(0)`                       | `0`     |
| `get_doubler()(-5)`                      | `-10`   |
| `apply(get_doubler(), 7)`                | `14`    |
| `apply([](int x) { return x + 1; }, 9)`  | `10`    |

## Run

```sh
./verify.sh
```

## Hints

- `get_doubler` body:
  ```cpp
  return [](int x) { return x * 2; };
  ```
  The conversion happens at the `return` because the function's
  declared return type (`int (*)(int)`) drives the implicit
  conversion of the lambda.

- `apply` body: `return fn(x);`. Trivial.

- The reading-mostly part of this step is the `apply(get_doubler(), 7)`
  expression — it threads the lambda-to-fnptr conversion through
  a function call boundary.

- Test 5 uses an inline captureless lambda directly at the call
  site of `apply`, demonstrating that lambda-to-fnptr is automatic.

- If you try to put captures `[x]` in the lambda inside
  `get_doubler`, the conversion fails to compile — that's the
  "no captures" rule.
