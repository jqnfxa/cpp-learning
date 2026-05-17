# 13 — Lambda capture of `*this`

> Spec: <https://en.cppreference.com/cpp/language/lambda>

## What you learn

C++11/14 had `[this]` (captures the pointer) and `[&]`
(implicit by-ref). Neither survives the enclosing object's
destruction.

C++17 added **`[*this]`**: copy the enclosing object into
the closure. The closure owns its own copy, independent of
the original.

```cpp
class widget {
    int x_;
public:
    auto make_getter() const {
        return [*this]() { return x_; };  /* closure holds a COPY of *this */
    }
};
```

After `widget w(42); auto g = w.make_getter(); w.set(999);`
the closure `g()` returns `42` (the captured snapshot), not
`999`.

## Task

Replace `[this]` with `[*this]` in `make_getter`. The
closure body stays the same; the capture mode changes from
pointer to by-value.

## Run

```sh
./verify.sh
```

## Hints

- `[*this]` is C++17 syntax. Inside the closure body, `x_`
  refers to the COPIED object's `x_`, not the original's.

- The use case is async work: pass a lambda to a thread or
  callback, and the original object can die while the
  closure runs.

- The copy is made AT THE CAPTURE EXPRESSION (when the
  lambda is constructed). Modifications to the original
  after that don't affect the closure.

- The closure is the size of `*this` -- could be expensive
  for large objects. Use `[this]` if you trust the lifetime
  alignment.

- Tests verify: getter holds the value, getter preserves
  value after mutation of original, multiple captures from
  one object are independent snapshots.
