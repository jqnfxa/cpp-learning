# 77 — `emplace` / `emplace_back` / `emplace_front`

> Spec: <https://en.cppreference.com/cpp/container/vector/emplace_back>

## What you learn

The `emplace*` family of container methods construct an element
**in place** inside the container, forwarding the arguments
directly to the element type's constructor. They were added to
C++11 alongside variadic templates and `std::forward` — both
required to implement perfect forwarding to an arbitrary
constructor.

The contrast with `push_back` is the headline:

```cpp
struct T { T(int, std::string); };

std::vector<T> v;
v.push_back(T(1, "x"));     /* 1 value ctor + 1 move ctor */
v.emplace_back(1, "x");     /*       1 value ctor only       */
```

The `push_back` line constructs a temporary `T`, then moves
(or copies, if `T` isn't movable) it into the vector. The
`emplace_back` line forwards `(1, "x")` to `T`'s constructor
*inside* the storage the vector reserves — no temporary, no
move. One construction. Same memory.

The other variants:

- `emplace(pos, args...)` — insert at iterator `pos`. Available
  on `vector`, `deque`, `list`, both ordered and unordered
  associative containers.
- `emplace_front(args...)` — `deque`, `list`, `forward_list`
  (`emplace_after`).

Each one takes the same `(args...)` shape and forwards via
`std::forward<Args>(args)...` to the element constructor.

### When `emplace_back` matters

For trivial / cheap types like `int`, `push_back` and
`emplace_back` produce equivalent code (the "move" is a register
copy). For *expensive* element types — strings, vectors of
strings, custom RAII handles — saving a move per insertion
matters in tight loops.

For *non-movable* element types (e.g. types with deleted move
ctor or with strong copy semantics only), `push_back(T(args))`
becomes a copy and `emplace_back(args...)` saves you that copy
entirely.

There's also the readability angle: `emplace_back(1, "x", 3.14)`
makes "what is being constructed" clearer than `push_back(T{1,
"x", 3.14})` when `T` is long-named.

### The `push_back({...})` wrinkle

```cpp
v.push_back({1, "x"});
```

This works for aggregate-like types because of *braced-init-
list to T* conversion. The braced list is converted to a `T`,
then that temporary is moved in. So `push_back({...})` is
*also* one ctor + one move, just spelled differently. It's not
the same as `emplace_back(1, "x")`.

### When `emplace_back` is a trap

You can't pass an `explicit` constructor's arguments through
`push_back({...})`, but you *can* through `emplace_back(...)`.
Some people find that uncomfortable: `emplace_back(...)` will
happily invoke explicit constructors. The fix is to read what
you're constructing into.

```cpp
struct Box { explicit Box(int); };

std::vector<Box> v;
v.push_back({1});            /* fails -- {1} -> Box requires implicit ctor */
v.emplace_back(1);            /* works -- but is that what you meant? */
```

## Task

In `solution.hpp`, a `Tracker` type with counters for value
constructor / copy ctor / move ctor invocations is provided.
Implement three factory functions that each insert one
`Tracker(n, name)` into a fresh container, using a different
construction idiom in each:

```cpp
namespace task77
{
    /* v.push_back(Tracker(n, name));   -> 1 value_ctor + 1 move_ctor */
    std::vector<Tracker> via_push_back(int n, std::string name);

    /* v.emplace_back(n, name);          -> 1 value_ctor, no moves */
    std::vector<Tracker> via_emplace_back(int n, std::string name);

    /* d.emplace_front(n, name);         -> 1 value_ctor, no moves */
    std::deque<Tracker> via_emplace_front(int n, std::string name);
}
```

Tests reset the counters before each call and assert the exact
construction counts.

## Run

```sh
./verify.sh
```

## Hints

- All three implementations are one line plus a `return`. Reset
  is automatic before each test via the fixture, not your job.

- For `via_push_back`, the temporary in the `push_back` argument
  is constructed in your function's scope, then moved in by the
  vector. You can spell the temporary either way:
  `v.push_back(Tracker(n, name))` or
  `v.push_back(Tracker{n, name})`.  
  Both give the same count: 1 value + 1 move.

- For `via_emplace_back`, write `v.emplace_back(n, name);`. The
  variadic perfect-forwarding inside `emplace_back` is what
  makes this one-construction.

- `emplace_front` is the same shape on a `deque`: `d.emplace_front(n, name);`.

- The tests don't check what happens to the `name` argument
  *after* the call. The implementation is free to move `name`
  into the tracker (which is what the value constructor does
  via `std::move` internally) — that's an implementation choice
  that doesn't affect the counters in this scaffold.

- A `vector<Tracker>` may reserve capacity > 1 on first insert,
  but the move-vs-emplace count is per-call and not affected by
  the eventual reallocation -- this scaffold only inserts one
  element so no rehash happens.

- `emplace_back` was specifically a C++11 addition because it
  needs *variadic templates* + *`std::forward`* to forward
  arguments perfectly. Both arrived in C++11; the `emplace*`
  family came along the same year. C++17 made `emplace_back`
  return a reference to the constructed element (it returned
  `void` in C++11/14).
