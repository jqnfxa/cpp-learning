# 4 — `if` and `switch` with init-statement

> Spec: <https://en.cppreference.com/cpp/language/if>

## What you learn

C++17 adds an **init-statement** to both `if` and `switch`:

```cpp
if (init-statement; condition) { ... }
switch (init-statement; expression) { ... }
```

The init-statement runs first; the condition (or switch
expression) sees names declared in it. The init-declared
names live for the duration of the if/else (or switch)
bodies, then go out of scope.

### The classic motivating use

Map lookup:

```cpp
/* C++14 form -- iterator name leaks into enclosing scope */
auto it = m.find(k);
if (it != m.end()) {
    use(it->second);
}
/* `it` is still in scope here -- pollutes the function */

/* C++17 form -- name scoped to the if */
if (auto it = m.find(k); it != m.end()) {
    use(it->second);
}
/* `it` is gone */
```

Two wins:

1. **Tighter scope**: the iterator name doesn't pollute the
   enclosing function. If you wrote another `find` later,
   you can reuse the name without ambiguity.
2. **Locality**: declaration and use are adjacent.

### Combined with structured bindings

`std::map::insert` returns a pair; structured binding +
init-statement is the canonical idiom:

```cpp
if (auto [it, inserted] = m.insert({k, v}); inserted) {
    /* newly added */
} else {
    /* already existed; `it` points to it */
}
```

### Initializing AND checking a return value

For functions that return an "object + bool" pair (or a
"valid-or-not" result), the init form is natural:

```cpp
if (auto result = parse(input); result.valid) {
    return result.value;
}
```

vs

```cpp
auto result = parse(input);
if (result.valid) {
    return result.value;
}
```

The C++17 form keeps `result` localized to where it's
relevant.

### The init-statement is a **statement**

Not an expression. It can declare multiple variables, do
side effects, be an expression statement:

```cpp
if (int x = 5, y = 10; x + y > 0) { ... }   /* multiple decls */
if (some_function(); other_function()) { ... } /* two expression statements */
```

The semicolon between init and condition is part of the
grammar.

### Switch with init

Same idea, slightly less common in practice:

```cpp
switch (auto& cache = get_cache(); cache.state) {
    case state::idle: ...
    case state::busy: ...
}
```

The cache reference is available in every case body, and
disappears after the switch.

### Scope rules

The init-declared names are scoped to:

- The condition.
- The `then` clause.
- The `else` clause (or all case bodies for switch).

They go out of scope after the if/else (or switch) ends:

```cpp
if (int x = 5; x > 0) {
    /* x in scope */
} else {
    /* x in scope */
}
/* x out of scope here */
```

### Doesn't apply to `while` / `for`

C++17 added init-statements to `if` and `switch`. For `for`
loops, the init was always there (`for (init; cond; step)`).
For `while`, there's no init form -- you'd use the
declared-while form `while (auto x = expr; cond)`... no,
actually `while` doesn't get init-statement in C++17. You'd
use a manual scope:

```cpp
{
    auto x = expr;
    while (cond) { ... }
}
```

C++20 added init-statement to range-for:

```cpp
for (auto v = make_vector(); auto& x : v) { ... }   /* C++20 */
```

### Comparison with the dedicated form

```cpp
/* C++17 if-init form */
if (auto p = find(); p != nullptr) {
    use(p);
}

/* C++14 manual scope form */
{
    auto p = find();
    if (p != nullptr) {
        use(p);
    }
}
```

The C++17 form is shorter, doesn't introduce a redundant
`{ }`, and reads naturally left-to-right.

### Style: when to use it

Use `if`-init whenever the init is **specifically for this
check** and you don't need the variable afterward. If you
do need the variable later, the C++14 pre-declaration form
is still correct.

Common cases:
- Map/set find + dereference.
- Insert + check-if-new.
- Parse + check-valid.
- Lock + check-held (with try_lock).

## Task

In `solution.hpp`, fill in three functions using `if`-init
or `switch`-init:

```cpp
inline int lookup_or_default(const std::map<int, int>& m, int k, int default_value) {
    if (auto it = m.find(k); it != m.end()) {
        return it->second;
    }
    return default_value;
}

inline int lookup_doubled(const std::map<int, int>& m, int k) {
    if (auto it = m.find(k); it != m.end()) {
        return it->second * 2;
    }
    return 0;
}

inline std::string classify(int x) {
    switch (int v = x; v <= 0 ? 0 : v < 10 ? 1 : 2) {
        case 0: return "non-positive";
        case 1: return "small";
        case 2: return "large";
    }
    return "?";
}
```

## Run

```sh
./verify.sh
```

## Hints

- The grammar: `if (init; condition) body`. The semicolon
  separates the init from the condition; both end with a
  closing paren before the body.

- `m.find(k)` returns an iterator. `auto it = m.find(k)`
  is the init; `it != m.end()` is the condition.

- `it->second` is the value field of the map's
  pair-element.

- For `classify`, the init `int v = x` introduces `v` as
  a local. The switch expression `v <= 0 ? 0 : v < 10 ? 1 : 2`
  reduces to a small int (0, 1, or 2), then matches a
  case label.

- The switch's init form is less ergonomic for this case
  than a regular if-else chain -- the example just
  demonstrates the syntax exists. In real code, prefer the
  clearer form (here, an if-chain).

- The init-declared variable is scoped to the if/switch.
  Trying to use it afterward is a compile error.

- Tests verify: lookup present at multiple keys, lookup
  missing returns default (and zero), empty map returns
  default, doubled-result correctness, classify across all
  three buckets including boundaries (9 -> small, 10 ->
  large, 0 / -5 -> non-positive).

- For `find`-and-check, this is the idiomatic C++17 form.
  Use it wherever the iterator is needed only inside the
  guard.

- C++20 added the init form to range-for too. For C++17,
  if/switch is the boundary.
