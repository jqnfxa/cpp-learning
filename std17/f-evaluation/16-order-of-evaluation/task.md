# 16 — New order-of-evaluation rules

> Spec: <https://en.cppreference.com/cpp/language/eval_order>

## What you learn

C++17 added specific sequencing guarantees that previously
were "unspecified evaluation order." The changes:

**Sequenced in C++17:**

- `a.b`, `a->b` — LHS evaluated before access.
- `a[b]` — `a` before `b`.
- `a << b`, `a >> b` — left-to-right.
- `a = b` — RHS before LHS storage.
- `a.b(args...)` — `a` before `b`'s arguments.
- `(*p)(args...)` — `*p` before `args...`.

**Still unsequenced:**

- Function argument order: `f(g(), h())` — `g()` and `h()`
  may run in either order. THIS IS THE TRAP from the
  C++14 `f(unique_ptr<A>(new A), unique_ptr<B>(new B))`
  leak hazard. C++17 didn't fix it (use `make_unique`).

- Multiple modifications of the same object in one
  expression: `a++ + ++a` — undefined behavior.

## Task

Implement `is_sequenced(scenario)` returning `true` for the
sequenced cases and `false` for the unsequenced ones (per
the table above).

## Run

```sh
./verify.sh
```

## Hints

- The function is `constexpr` and is a pure mapping.

- The reference body is one big `||` chain.

- `function_arg_order` is the famous C++14 leak hazard --
  C++17 didn't sequence function arguments. Use
  `std::make_unique` / `std::make_shared` factories.

- `multiple_modifications` is still UB in C++17. C++20
  didn't change it either.

- This is a read-only conceptual task; the heavy lifting is
  in this task.md.
