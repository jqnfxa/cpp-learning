# 15 — Nested namespaces

> Spec: <https://en.cppreference.com/cpp/language/namespace>

## What you learn

C++17 lets you collapse nested namespace declarations:

```cpp
/* C++14 */
namespace a {
namespace b {
namespace c {
    int x = 42;
}
}
}

/* C++17 */
namespace a::b::c {
    int x = 42;
}
```

Same semantics, much less indentation.

### Limits

- The `::`-separator form opens all the namespaces at once;
  you can't split across multiple files (you can, but each
  file must use the same form -- nested or not is your
  choice per file).

- Inline namespaces in C++17 form: `namespace a::inline b::c
  { ... }` (C++20).

## Task

Inside `namespace task15::deep::nest { ... }`, implement
`answer()` to return 42 and `label()` to return "deep-nest".

## Run

```sh
./verify.sh
```

## Hints

- The namespace declaration is `namespace task15::deep::nest`.
  Everything inside is implicitly in `task15::deep::nest::`.

- Call sites use the full path:
  `task15::deep::nest::answer()` or via a namespace alias
  `namespace dn = task15::deep::nest;`.

- The `using task15::deep::nest::answer;` form imports just
  one name.

- This is syntactic sugar -- the compiled output is
  identical to the C++14 nested-blocks form.
